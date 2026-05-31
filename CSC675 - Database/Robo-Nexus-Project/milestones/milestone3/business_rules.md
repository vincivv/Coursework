# AI-Powered Humanoid Robot Fleet Management System

## Milestone 3 – Phase 1: Business Rules Definition

---

## Rule 1: Minimum Battery to Execute Task

**Purpose:** Prevent robots from executing tasks when battery is critically low.

**Description:** A robot shall not start a task execution if its battery level is below 20%.

**Challenges:** Checking latest timestamp battery level per robot.

**Assumptions:** Latest EnergyStatus record represent current battery.

**Planned Approach:** Trigger on task execution.

---

## Rule 2: Charging Station Capacity Enforcement

**Purpose:** Prevent from overloading the charging stations.

**Description:** The number of active Charging Sessions (robots) in charging stations shall not exceed its capacity.

**Challenges:** Count the number of active sessions and the capacity of the stations.

**Assumptions:** Active session records represent the latest number of robots at charging stations.

**Planned Approach:** Trigger on ChargingStationsSessions.

---

## Rule 3: One active task per Robot

**Purpose:** Prevents overloading robots.

**Description:** A robot can only have one active task at a time and must finish the task before getting another active task.

**Challenges:** Can cause race conditions with other robots trying to take roles that are dependent on other roles.

**Assumptions:** A robot can not be multitasking.

**Planned Approach:** Triggers on Task Execution.

---

## Rule 4: Safe Task Scheduling and Robot Coordination

**Purpose:** Ensure that robots execute tasks in a coordinated and conflict-free manner within shared environments.

**Description:** The system must manage task assignments so that each robot is scheduled with a start time that does not conflict with its own existing tasks. A robot shall not begin a new task if it already has an active task in progress. Task execution must be coordinated to prevent overlapping operations for the same robot.

**Challenges:** Each task execution has a defined start time and end time. A task is considered active if it has started but not yet completed.

**Assumptions:** Robots operate in shared environments where conflicts are possible. Each task has a defined time window and location. Robot assignments and schedules are stored in the database.

**Planned Approach:** Use a stored procedure to assign tasks that checks for scheduling conflicts before inserting a new task execution record. Implement triggers to validate that new or updated task schedules do not overlap with existing ones for the same robot or location. Use helper functions to detect time and location conflicts between tasks.

---

## Rule 5: Maintenance Restricts Operational Activity

**Purpose:** Prevent unsafe operation of robots undergoing maintenance.

**Description:** The system restricts task execution for robots that have an open or active MaintenanceRecord.

**Challenges:** The database must determine whether a maintenance period is currently active.

**Assumptions:** MaintenanceRecord includes timestamps that indicate maintenance duration.

**Planned Approach:** Use a trigger on TaskExecution to validate maintenance status before allowing execution.

---

## Rule 6: Robot cannot charge and execute a task at the same time

**Purpose:** Prevents robots from having multiple states at once.

**Description:** A robot can not be doing a task while charging at the same time.

**Challenges:** A robot might need to start charging while still working on a task.

**Assumptions:** A robot must stop a task execution before starting to charge.

**Planned Approach:** Triggers on TaskExecutions and ChargingSessions.

---

## Rule 7: Single Active Role per Robot

**Purpose:** Ensure operational clarity and prevent conflicting behaviour by restricting each robot to one active role at a time.

**Description:** Single role for the robot so that robot cannot hold more than one active role simultaneously. When a new role assignment is recorded for a robot, any previously active role must be completed, deactivated, or replaced before the new role becomes effective. This prevents ambiguity in robot responsibilities and maintains consistent task coordination.

**Challenges:** Role records may be updated or inserted over time. The database must determine which role is currently active and prevent overlapping active role assignments for the same robot.

**Assumptions:** Robot roles are stored in the RobotRole table. A role can be identified as active based on status or timestamp attributes.

**Planned Approach:** Use a trigger on RobotRole insertion or update that checks for existing active role records for the same robot. If another active role exists, the operation is rejected or the previous role is automatically deactivated before allowing the new assignment.

---

## Rule 8: Automatic Alert on Critical Battery Level

**Purpose:** Ensure low-energy conditions are recorded immediately.

**Description:** If a robot’s battery level falls below a defined threshold, an alert record is automatically generated.

**Challenges:** Battery values change frequently and must be monitored consistently.

**Assumptions:** EnergyStatus records battery levels with timestamps.

**Planned Approach:** Use a trigger on EnergyStatus insert/update to create an Alert when battery level < threshold.

---

## Rule 9: Task Execution Requires Valid Assignment

**Purpose:** Ensure that robots only execute tasks that have been formally assigned and authorized within the system.

**Description:** The system must restrict task execution to tasks that have an existing and valid assignment record. A robot cannot begin execution of a task unless that task is associated with a corresponding Task assignment entry linking the robot and the task. This ensures controlled task management within the fleet.

**Challenges:** Task execution records may be inserted directly without verifying whether a valid assignment exists. The database must validate the relationship between Robot, Task, and TaskAssignment before allowing execution to proceed.

**Assumptions:** All valid task assignments are recorded in the TaskAssignment table prior to execution. A task execution is considered valid only if a matching assignment exists for the same robot and task.

**Planned Approach:** Implement an insert trigger on the TaskExecution table that checks for the existence of a corresponding TaskAssignment record for the same robot and task. If no valid assignment is found, the operation is rejected. A helper function may be used to verify assignment validity.

---

## Rule 10: Robot performance record and diagnostic report

**Purpose:** Ensure that declining robot performance is automatically detected and formally documented for further analysis.

**Description:** The system must monitor performance metrics recorded for each robot. When a newly inserted performance metric falls below a defined acceptable threshold for a given metric type, a corresponding diagnostic report is automatically generated for that robot. This ensures that abnormal or degraded performance conditions are not overlooked and are formally tracked within the system.

**Challenges:** Performance metrics may vary by metric type and threshold values may differ depending on operational context. The database must evaluate the inserted metric value against the appropriate threshold and determine whether a diagnostic report already exists for the same condition to avoid duplicate reports.

**Assumptions:** Performance metrics are recorded in the PerformanceMetric table with a metric type, value, and timestamp. Threshold values are predefined and consistently applied for each metric type. Diagnostic reports are stored in the DiagnosticReport table and associated with a specific robot.

**Planned Approach:** Implement an insert trigger on the PerformanceMetric table that compares the new metric value against the defined threshold. If the value falls below the acceptable limit, the trigger inserts a new record into the DiagnosticReport table for the corresponding robot. A helper function may be used to evaluate threshold logic and prevent duplicate reports within a defined time window.

---

## Rule 11: Support Request on Critical Alert

**Purpose:** Ensure that critical robot issues are formally tracked and escalated for resolution.

**Description:** When an alert with severity marked as “Critical” is recorded for a robot, the system automatically creates a corresponding SupportRequest linked to that robot. This ensures that serious operational issues are not ignored and are formally assigned for review and resolution.

**Challenges:** The system must detect the severity level at the time of alert creation and prevent duplicate support requests for the same unresolved issue.

**Assumptions:** Alert records include a severity attribute. A SupportRequest represents a formal record of required maintenance. Technicians are registered in the system and may later be assigned to the support request.

**Planned Approach:** Implement an insert trigger on the Alert table. When a new alert is inserted with high severity, the trigger automatically inserts a corresponding record into the SupportRequest table.

---

## Rule 12: Emotional record mapping only during interaction session

**Purpose:** Ensure emotional data is recorded only within valid robot-user interaction periods.

**Description:** An EmotionalRecord can only be created if it is associated with an existing and active InteractionSession. The timestamp of the emotional record must fall within the start_time and end_time of the corresponding interaction session. This ensures emotional data reflects actual interaction activity rather than standalone or invalid entries.

**Challenges:** The database must verify temporal consistency by comparing the emotional record timestamp against the interaction session’s time window. It must also ensure that the referenced interaction session exists.

**Assumptions:** InteractionSession stores start_time and end_time. EmotionalRecord includes a timestamp and references an interaction_id. An interaction is considered valid only within its defined time range.

**Planned Approach:** Use a trigger on the EmotionalRecord table to validate temporal and referential consistency before allowing a new record to be created. The trigger checks that the referenced InteractionSession exists and retrieves its start_time and end_time values. It then compares the EmotionalRecord timestamp against this time window. If the emotional record timestamp falls outside the interaction session’s defined time range, the operation is rejected. This ensures emotional data is recorded only during valid interaction periods and maintains temporal integrity within the database.

---

## Rule 13: Single Active AI Request per Robot

**Purpose:** Prevent overlapping AI processing requests for the same robot and maintain clear decision flow when required.

**Description:** At any given time, a robot can have only one active AIRequest record. A new AI Request for a robot cannot be created if another request for the same robot is still pending or incomplete.

**Challenges:** To make the pipeline to get current active AIRequest and associated AIModel.

**Assumptions:** AI Request includes information to determine whether a request is associated with one model.

**Planned Approach:** Use an insert trigger on the AIRequest table to check whether an active request already exists for the same robot.

---

## Rule 14: Technician specialization and robot Model

**Purpose:** Ensure that maintenance tasks are performed only by technicians who are qualified to service a specific robot model.

**Description:** When a MaintenanceRecord is created, the associated technician has to service the robot. The database prevents assigning wrong technicians.

**Challenges:** The database must validate that the technician has specialization for the robot model. This requires checking relationships between Technician, Robot, and RobotModels tables.

**Assumptions:** Technician specializations are stored in an associative table. Technicians qualifications are maintained and up to date in the database.

**Planned Approach:** Insert trigger on the MaintenanceRecord table. The trigger retrieves the robot's model and verifies that the technician is associated with that model in the specialization table.

---

## Rule 15: Robot Model Deployment compatibility

**Purpose:** Ensure that different deployment locations operate only with compatible robot models and approved software.

**Description:** When a robot is assigned to a deployment location, the database validates that the robot model is supported in that specific environment type and that the installed software is approved for that model within that environment. If the combination of location type, robot model, and software is not right, the assignment of the robot to that location is rejected.

**Challenges:** The validation requires checking across multiple related tables which may increase the complexity.

**Assumptions:** Certain deployment environments require specific robot models and approved software configurations. For example, hospitals, warehouses, and homes may have different requirements. Each robot model is associated with one or more compatible software and deployment locations define operational constraints based on environment type.

**Planned Approach:** Implement a stored procedure for robot registration or deployment that validates compatibility between the robot model, software versions and deployment location type before inserting.
