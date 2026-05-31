USE robo_nexus;

DELIMITER $$

/*
Rule 1: Minimum Battery to Execute Task.
Purpose: Prevent robots from executing tasks when battery is critically low.
Description: A robot shall not start a task execution if its battery level is below 20%.
Challenges: checking latest timestamp battery level per robot
Assumptions: Latest EnergyStatus record represent current battery
Planned Approach: Trigger on task execution
*/

DROP TRIGGER IF EXISTS trg_check_battery_before_task_execution $$

CREATE TRIGGER trg_check_battery_before_task_execution
    BEFORE INSERT ON TaskExecutions
    FOR EACH ROW
BEGIN
    DECLARE v_robot_id INT;
    DECLARE v_battery INT;

    -- Get robot assigned to this task
    SELECT RobotID
    INTO v_robot_id
    FROM TaskAssignments
    WHERE TaskID = NEW.TaskID
    ORDER BY AssignedAt DESC
    LIMIT 1;

    -- Get latest battery level
    SELECT BatteryLevel
    INTO v_battery
    FROM EnergyStatus
    WHERE RobotID = v_robot_id
    ORDER BY RecordedAt DESC
    LIMIT 1;

    IF v_battery < 20 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cannot start task execution: robot battery is below 20%.';
    END IF;

END $$


/*
Rule 2: Charging Station Capacity Enforcement
Purpose: Prevent from overloading the charging stations.
Description: The number of active Charging Sessions (robots) in charging stations shall not exceed its capacity.
Challenges: Count the number of active sessions and the capacity of the stations.
Assumptions: Active session records represent the latest number of robots at charging stations.
Planned Approach: Trigger on ChargingStationsSessions.
*/

DROP TRIGGER IF EXISTS trg_check_station_capacity_before_insert $$

CREATE TRIGGER trg_check_station_capacity_before_insert
    BEFORE INSERT ON ChargingStationSessions
    FOR EACH ROW
BEGIN
    DECLARE v_capacity INT;
    DECLARE v_active_count INT;

    -- Get station capacity
    SELECT Capacity
    INTO v_capacity
    FROM ChargingStations
    WHERE ChargingStationID = NEW.ChargingStationID;

    -- Count active charging sessions
    SELECT COUNT(*)
    INTO v_active_count
    FROM ChargingStationSessions css
             JOIN ChargingSessions cs
                  ON css.ChargingSessionID = cs.ChargingSessionID
    WHERE css.ChargingStationID = NEW.ChargingStationID
      AND cs.EndTime IS NULL;

    IF v_active_count >= v_capacity THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Charging station capacity exceeded.';
    END IF;

END $$


/*
Rule 3: One active task per Robot
Purpose: Prevents overloading robots.
Description: A robot can only have one active task at a time and must finish the task before getting another active task.
Challenges: Can cause race conditions with other robots trying to take roles that are dependent on other roles.
Assumptions: a robot can not be multitasking.
Planned Approach: Triggers on Task Execution.
*/

DROP TRIGGER IF EXISTS trg_rule3_one_active_task $$

CREATE TRIGGER trg_rule3_one_active_task
    BEFORE INSERT ON TaskExecutions
    FOR EACH ROW
BEGIN
    DECLARE v_robot_id INT;
    DECLARE v_count INT;

    -- Get robot assigned to this task
    SELECT RobotID
    INTO v_robot_id
    FROM TaskAssignments
    WHERE TaskID = NEW.TaskID
    ORDER BY AssignedAt DESC
    LIMIT 1;

    IF v_robot_id IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Rule 3 violation: No robot assigned to this task.';
    END IF;

    -- Check for active task (Status = 0)

    SELECT COUNT(*)
    INTO v_count
    FROM TaskExecutions te
             JOIN TaskAssignments ta ON te.TaskID = ta.TaskID
    WHERE ta.RobotID = v_robot_id
      AND te.Status = 0;

    IF v_count > 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Rule 3 violation: Robot already has an active task.';
    END IF;
END $$




/*
Rule 4: Safe Task Scheduling and Robot Coordination
Purpose: Ensure that robots execute tasks in a coordinated and conflict-free manner within shared environments.
Description: The system must manage task assignments so that each robot is scheduled with a start time that does not conflict with its own existing tasks. A robot shall not begin a new task if it already has an active task in progress. Task execution must be coordinated to prevent overlapping operations for the same robot.
Challenges:Each task execution has a defined start time and end time. A task is considered active if it has started but not yet completed.
Assumptions: Robots operate in shared environments where conflicts are possible. Each task has a defined time window and location. Robot assignments and schedules are stored in the database.
Planned Approach:Use triggers to validate that new or updated task schedules do not overlap with existing ones for the same robot.
*/

DROP TRIGGER IF EXISTS trg_rule4_safe_task_scheduling $$

CREATE TRIGGER trg_rule4_safe_task_scheduling
    BEFORE INSERT ON TaskExecutions
    FOR EACH ROW
BEGIN
    DECLARE v_robot_id INT;
    DECLARE v_conflict_count INT;

    -- Ensure start and end time exist
    IF NEW.StartTime IS NULL OR NEW.EndTime IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'StartTime and EndTime must be provided.';
    END IF;

    -- Get robot assigned to this task
    SELECT RobotID
    INTO v_robot_id
    FROM TaskAssignments
    WHERE TaskID = NEW.TaskID
    ORDER BY AssignedAt DESC
    LIMIT 1;

    -- Check overlapping schedule

    SELECT COUNT(*)
    INTO v_conflict_count
    FROM TaskExecutions te
             JOIN TaskAssignments ta
                  ON te.TaskID = ta.TaskID
    WHERE ta.RobotID = v_robot_id
      AND NEW.StartTime < IFNULL(te.EndTime,'9999-12-31 23:59:59')
      AND NEW.EndTime > te.StartTime;
    IF v_conflict_count > 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Rule 4 violated: task schedule overlaps with another task.';
    END IF;
END $$


/*
Rule 5: Maintenance Restricts Operational Activity
Purpose: Prevent unsafe operation of robots undergoing maintenance
Description: The system restricts task execution for robots that have an open or active MaintenanceRecord.
Challenges: The database must determine whether a maintenance period is currently active
Assumptions: MaintenanceRecord includes timestamps that indicate maintenance duration.
Planned Approach: Use a trigger on TaskExecution to validate maintenance status before allowing execution.
*/

DROP TRIGGER IF EXISTS trg_rule5_maintenance_blocks_task $$

CREATE TRIGGER trg_rule5_maintenance_blocks_task
    BEFORE INSERT ON TaskExecutions
    FOR EACH ROW
BEGIN
    DECLARE v_robot_id INT;
    DECLARE v_maintenance_count INT;

    -- Get robot assigned to this task
    SELECT RobotID
    INTO v_robot_id
    FROM TaskAssignments
    WHERE TaskID = NEW.TaskID
    ORDER BY AssignedAt DESC
    LIMIT 1;

    -- Check if robot has any maintenance records
    SELECT COUNT(*)
    INTO v_maintenance_count
    FROM MaintenanceRecord
    WHERE RobotID = v_robot_id;

    IF v_maintenance_count > 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Rule 5 violated: robot is currently under maintenance.';
    END IF;

END $$


/*
Rule 6: Robot cannot charge and execute a task at the same time
Purpose: Prevents robots from having multiple states at once.
Description: A robot can not be doing a task while charging at the same time.
Challenges: A robot might need to start charging while still working on a task.
Assumptions: A robot must stop a task execution before starting to charge.
Planned Approach: triggers on TaskExecutions and ChargingSessions.
*/

DROP TRIGGER IF EXISTS trg_rule6_no_task_while_charging $$

CREATE TRIGGER trg_rule6_no_task_while_charging
    BEFORE INSERT ON TaskExecutions
    FOR EACH ROW
BEGIN
    DECLARE v_robot_id INT;
    DECLARE v_active_charge INT;

    -- Get robot assigned to this task
    SELECT RobotID
    INTO v_robot_id
    FROM TaskAssignments
    WHERE TaskID = NEW.TaskID
    ORDER BY AssignedAt DESC
    LIMIT 1;

    -- Check if robot is actively charging
    SELECT COUNT(*)
    INTO v_active_charge
    FROM ChargingSessions cs
             JOIN ChargingStationSessions css
                  ON cs.ChargingSessionID = css.ChargingSessionID
             JOIN RobotChargingStations rcs
                  ON rcs.ChargingStationID = css.ChargingStationID
    WHERE rcs.RobotID = v_robot_id
      AND cs.EndTime IS NULL;
    IF v_active_charge > 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Rule 6 violated: robot cannot execute a task while charging.';
    END IF;
END $$
/* ============================================================
Rule 7: Single Active Role per Robot
Purpose: Ensure operational clarity and prevent conflicting behaviour by restricting each robot to one active role at a time.
Description: A robot cannot hold more than one role simultaneously. A role must be explicitly changed rather than duplicated.
Challenges: Prevent unnecessary duplicate role assignments or null role updates.
Assumptions: Each robot stores its current role in Robots.RobotRoleID.
Planned Approach: Trigger on Robots to validate role updates.
============================================================ */

DROP TRIGGER IF EXISTS trg_rule7_single_active_role $$

CREATE TRIGGER trg_rule7_single_active_role
    BEFORE UPDATE ON Robots
    FOR EACH ROW
BEGIN
    -- Prevent removing role
    IF NEW.RobotRoleID IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Robot must have a valid role assigned.';
    END IF;

    -- Prevent assigning same role again
    IF OLD.RobotRoleID = NEW.RobotRoleID THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Robot already has this role assigned.';
    END IF;

END $$


/* ============================================================
Rule 8: Automatic Alert on Critical Battery Level
Purpose: Ensure low-energy conditions are recorded immediately.
Description: If a robot’s battery level falls below a defined threshold, a critical alert is automatically generated.
Challenges: Avoid generating duplicate alerts in short time window.
Assumptions: EnergyStatus stores BatteryLevel and Alerts stores severity and timestamp.
Planned Approach: Trigger on EnergyStatus.
============================================================ */

DROP TRIGGER IF EXISTS RULE8_CRITICAL_BATTERY_ALERT_TRG $$

CREATE TRIGGER RULE8_CRITICAL_BATTERY_ALERT_TRG
    AFTER INSERT ON EnergyStatus
    FOR EACH ROW
BEGIN
    DECLARE recent_alert_exists BOOLEAN DEFAULT FALSE;

    IF NEW.BatteryLevel < 15 THEN

        SELECT EXISTS (
            SELECT 1
            FROM Alerts
            WHERE RobotID = NEW.RobotID
              AND Severity = 'Critical'
              AND TimeStamp >= (NOW() - INTERVAL 10 MINUTE)
        ) INTO recent_alert_exists;

        IF recent_alert_exists = FALSE THEN
            INSERT INTO Alerts (RobotID, Severity, TimeStamp)
            VALUES (NEW.RobotID, 'Critical', NOW());
        END IF;

    END IF;

END $$


/* ============================================================
Rule 9: Task Execution Requires Valid Assignment
Purpose: Ensure that robots only execute tasks that have been formally assigned.
Description: A task execution cannot be inserted unless a corresponding TaskAssignment exists for that RobotID and TaskID.
Challenges: Validate relationship between Robot, Task, and TaskAssignment before execution.
Assumptions: TaskAssignments links RobotID and TaskID.
Planned Approach: Trigger on TaskExecutions.
============================================================ */
DROP TRIGGER IF EXISTS RULE9_TASK_EXEC_REQUIRES_ASSIGNMENT_TRG $$

CREATE TRIGGER RULE9_TASK_EXEC_REQUIRES_ASSIGNMENT_TRG
    BEFORE INSERT ON TaskExecutions
    FOR EACH ROW
BEGIN
    DECLARE assignment_exists BOOLEAN DEFAULT FALSE;
    SELECT EXISTS (
        SELECT 1
        FROM TaskAssignments
        WHERE TaskID = NEW.TaskID
    ) INTO assignment_exists;
    IF assignment_exists = FALSE THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'No TaskAssignment exists for this TaskID.';
    END IF;
END $$

/*
Rule 10: Robot performance record and diagnostic report
Purpose: Ensure that declining robot performance is automatically detected and formally documented for further analysis
Description: The system monitors performance metrics recorded for each robot. When a newly inserted performance metric falls below a defined acceptable threshold for a given metric type, a corresponding diagnostic report is automatically generated for that robot.
Challenges: Performance metrics may vary by metric type and threshold values may differ depending on operational context.
Assumptions: PerformanceMetrics stores MetricType, MetricValue, and TimeStamp. DiagnosticReports are linked to Robots and AIModels.
Planned Approach: Use a threshold table, helper function, and AFTER INSERT trigger on PerformanceMetrics to generate a DiagnosticReport when needed.
*/

DROP TABLE IF EXISTS MetricThresholds $$

CREATE TABLE MetricThresholds (
                                  MetricType VARCHAR(45) NOT NULL PRIMARY KEY,
                                  ThresholdValue INT UNSIGNED NOT NULL
) $$

DROP FUNCTION IF EXISTS IS_LOW_PERFORMANCE_METRIC $$

CREATE FUNCTION IS_LOW_PERFORMANCE_METRIC(
    p_MetricType VARCHAR(45),
    p_MetricValue INT UNSIGNED
)
    RETURNS BOOLEAN
    DETERMINISTIC
BEGIN
    DECLARE threshold_val INT UNSIGNED;

    SELECT ThresholdValue
    INTO threshold_val
    FROM MetricThresholds
    WHERE MetricType = p_MetricType;

    RETURN p_MetricValue < threshold_val;
END $$

DROP TRIGGER IF EXISTS CREATE_DIAGNOSTIC_REPORT_TRG $$

CREATE TRIGGER CREATE_DIAGNOSTIC_REPORT_TRG
    AFTER INSERT ON PerformanceMetrics
    FOR EACH ROW
BEGIN
    DECLARE is_low BOOLEAN DEFAULT FALSE;
    DECLARE report_exists BOOLEAN DEFAULT FALSE;
    DECLARE selected_ai_model_id INT UNSIGNED;

    SET is_low = IS_LOW_PERFORMANCE_METRIC(NEW.MetricType, NEW.MetricValue);

    IF is_low THEN

        SELECT EXISTS (
            SELECT 1
            FROM DiagnosticReports
            WHERE RobotID = NEW.RobotID
              AND GeneratedAt >= NEW.TimeStamp - INTERVAL 1 DAY
        ) INTO report_exists;

        IF NOT report_exists THEN

            SELECT AIModelID
            INTO selected_ai_model_id
            FROM AIModels
            ORDER BY AIModelID
            LIMIT 1;

            INSERT INTO DiagnosticReports (
                RobotID,
                AIModelID,
                GeneratedAt,
                Summary
            )
            VALUES (
                       NEW.RobotID,
                       selected_ai_model_id,
                       NEW.TimeStamp,
                       CONCAT(
                               'Performance issue detected for metric type ',
                               NEW.MetricType,
                               '. Recorded value: ',
                               NEW.MetricValue
                       )
                   );

        END IF;
    END IF;

END $$


/*
Rule 11 :Support Request on Critical Alert
Purpose: Ensure that critical robot issues are formally tracked and escalated for resolution.
Description: When an alert with severity marked as "Critical" is recorded for a robot, the system automatically creates a corresponding SupportRequest linked to that robot.
Challenges: Prevent duplicate support requests for the same issue.
Assumptions: Alerts contain Severity and TimeStamp. SupportRequests store RobotID and TimeReported.
Planned Approach: AFTER INSERT trigger on Alerts to generate SupportRequest when Severity = 'Critical'.
*/

DROP TRIGGER IF EXISTS CREATE_SUPPORT_REQUEST_TRG $$

CREATE TRIGGER CREATE_SUPPORT_REQUEST_TRG
    AFTER INSERT ON Alerts
    FOR EACH ROW
BEGIN
    DECLARE request_exists BOOLEAN DEFAULT FALSE;

    IF NEW.Severity = 'Critical' THEN

        SELECT EXISTS (
            SELECT 1
            FROM SupportRequests
            WHERE RobotID = NEW.RobotID
              AND TimeReported >= NEW.TimeStamp - INTERVAL 1 DAY
        ) INTO request_exists;

        IF NOT request_exists THEN
            INSERT INTO SupportRequests (
                RobotID,
                IssueDetails,
                TimeReported
            )
            VALUES (
                       NEW.RobotID,
                       CONCAT('Critical alert detected automatically from AlertID ', NEW.AlertID),
                       NEW.TimeStamp
                   );
        END IF;

    END IF;

END $$


/*
Rule 12 :Emotional record mapping only during interaction session
Purpose: Ensure emotional data is recorded only within valid robot-user interaction periods.
Description: An EmotionalRecord can only be mapped to an InteractionSession if the emotional timestamp falls within the session's start and end time.
Challenges: Must validate temporal consistency across tables.
Assumptions: EmotionalRecords contains Timestamp. InteractionSessions contains StartTIme and EndTIme.
Planned Approach: BEFORE INSERT trigger on EmotionalInteractionSessions to validate timestamp boundaries.
*/

DROP TRIGGER IF EXISTS VALIDATE_EMOTIONAL_INTERACTION_TRG $$

CREATE TRIGGER VALIDATE_EMOTIONAL_INTERACTION_TRG
    BEFORE INSERT ON EmotionalInteractionSessions
    FOR EACH ROW
BEGIN
    DECLARE emotional_ts DATETIME;
    DECLARE session_start DATETIME;
    DECLARE session_end DATETIME;

    SELECT `Timestamp`
    INTO emotional_ts
    FROM EmotionalRecords
    WHERE Emotional_ID = NEW.EmotionalID;

    SELECT StartTIme, EndTIme
    INTO session_start, session_end
    FROM InteractionSessions
    WHERE InteractionID = NEW.InteractionID;

    IF emotional_ts IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Invalid EmotionalRecord reference';
    END IF;

    IF session_start IS NULL OR session_end IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Invalid InteractionSession reference';
    END IF;

    IF emotional_ts < session_start OR emotional_ts > session_end THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Emotional record timestamp is outside the interaction session time window';
    END IF;

END $$

/*
Rule 13: Single Active AI Request per Robot
Purpose: Prevent overlapping AI processing requests for the same robot.
Description: At any given time, a robot can have only one active AIRequest record.
Challenges: The system must detect whether an existing AIRequest has not yet produced a corresponding AIResponse.
Assumptions: An AIRequest is considered active if no related AIResponse exists.
Planned Approach: BEFORE INSERT trigger on AIRequests to check for existing active request for the same RobotID.
*/

DROP TRIGGER IF EXISTS trg_single_active_ai_request $$

CREATE TRIGGER trg_single_active_ai_request
    BEFORE INSERT ON AIRequests
    FOR EACH ROW
BEGIN
    DECLARE active_count INT;

    SELECT COUNT(*)
    INTO active_count
    FROM AIRequests ar
             LEFT JOIN AIResponses res
                       ON ar.AIRequestID = res.AIRequestsID
    WHERE ar.RobotID = NEW.RobotID
      AND res.AIResponseID IS NULL;

    IF active_count > 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Only one active AI request per robot is allowed.';
    END IF;

END $$


/*
Rule 14: Technician Authorization by Deployment Location
Purpose: Ensure maintenance is performed only by technicians assigned to the same deployment location as the robot.
Description: When a MaintenanceRecord is created, the technician must belong to the same DeploymentLocation as the robot.
Challenges: Requires validation across Robots and Technicians tables.
Assumptions: Robots and Technicians both reference DeploymentLocationID.
Planned Approach: BEFORE INSERT trigger on MaintenanceRecord to compare DeploymentLocationID values.
*/

DROP TRIGGER IF EXISTS trg_technician_location_check $$

CREATE TRIGGER trg_technician_location_check
    BEFORE INSERT ON MaintenanceRecord
    FOR EACH ROW
BEGIN
    DECLARE robot_location INT;
    DECLARE technician_location INT;

    SELECT DeploymentLocationID
    INTO robot_location
    FROM Robots
    WHERE RobotID = NEW.RobotID;

    SELECT DeploymentLocationID
    INTO technician_location
    FROM Technicians
    WHERE TechnicianID = NEW.TechinicianID;

    IF robot_location IS NULL OR technician_location IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Invalid robot or technician reference.';
    END IF;

    IF robot_location <> technician_location THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Technician not authorized for this robot deployment location.';
    END IF;

END $$


/*
Rule 15: Robot Model and Deployment Location Compatibility
Purpose: Ensure that robot models are compatible with the environment type of the deployment location.
Description: When updating a robot’s DeploymentLocation, the system validates that the robot model is allowed in that environment type.
Challenges: Requires validation across Robots, RobotModels, and DeploymentLocations.
Assumptions: EnvironmentType is stored in DeploymentLocations and Model is stored in RobotModels.
Planned Approach: BEFORE UPDATE trigger on Robots to enforce environment-model compatibility.
*/

DROP TRIGGER IF EXISTS trg_model_deployment_check $$

CREATE TRIGGER trg_model_deployment_check
    BEFORE UPDATE ON Robots
    FOR EACH ROW
BEGIN
    DECLARE v_env VARCHAR(20);
    DECLARE v_model VARCHAR(20);

    SELECT EnvironmentType
    INTO v_env
    FROM DeploymentLocations
    WHERE DeploymentLocationID = NEW.DeploymentLocationID;

    SELECT Model
    INTO v_model
    FROM RobotModels
    WHERE RobotModelID = NEW.RobotModelID;

    IF v_env IS NULL OR v_model IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Invalid deployment location or robot model.';
    END IF;

    IF v_env = 'HOSPITAL' AND v_model <> 'Model1' THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Model not allowed in hospital environment.';
    END IF;

    IF v_env = 'WAREHOUSE' AND v_model <> 'Model2' THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Model not allowed in warehouse environment.';
    END IF;

END $$

DELIMITER ;
