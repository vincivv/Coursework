USE robo_nexus;

-- =========================
-- RULE 1 TEST
-- =========================

INSERT INTO EnergyStatus (ChargingSessionID, BatteryLevel, RobotID, RecordedAt)
VALUES
    (2, 15, 2, NOW() + INTERVAL 1 MINUTE),
    (3, 10, 3, NOW() + INTERVAL 2 MINUTE),
    (4, 5, 4, NOW() + INTERVAL 3 MINUTE);

-- Create a task execution while battery is low (Rule 1 violation)
INSERT INTO TaskAssignments (RobotID, UserID, TaskID, AssignedAt)
VALUES (2, 2, 2, NOW());

INSERT INTO TaskExecutions (TaskID, StartTime, EndTime, Status)
VALUES (2, NOW(), NOW() + 10, 1);

-- =========================
-- RULE 2 TEST
-- =========================

-- Step 1: create a dedicated test station with capacity 1
INSERT INTO ChargingStations (StationName, Capacity, Status)
VALUES ('Rule2 Test Station', 1, 'AVAILABLE');

SET @station_id = LAST_INSERT_ID();

-- Step 2: create first active charging session
INSERT INTO ChargingSessions (DeploymentLocationID, StartTime, EndTime)
VALUES (2, NOW(), NULL);

SET @session_1 = LAST_INSERT_ID();

-- Step 3: attach first active session to the station
-- SHOULD PASS
INSERT INTO ChargingStationSessions (ChargingSessionID, ChargingStationID)
VALUES (@session_1, @station_id);

-- Step 4: create second active charging session
INSERT INTO ChargingSessions (DeploymentLocationID, StartTime, EndTime)
VALUES (2, NOW() + INTERVAL 1 MINUTE, NULL);

SET @session_2 = LAST_INSERT_ID();

-- Step 5: attach second active session to the same station
-- SHOULD FAIL because capacity = 1 and one active session already exists
INSERT INTO ChargingStationSessions (ChargingSessionID, ChargingStationID)
VALUES (@session_2, @station_id);

-- =====================================
-- RULE 3 TEST
-- =====================================

-- Step 1: create two test tasks
INSERT INTO Tasks (TaskName, Description, Priority, Status)
VALUES
    ('Rule3 Test Task A', 'First task for active-task test', '2', 0),
    ('Rule3 Test Task B', 'Second task for active-task test', '2', 0);

SET @task_b = LAST_INSERT_ID();
SET @task_a = @task_b - 1;

-- Step 2: assign both tasks to the same robot
INSERT INTO TaskAssignments (RobotID, UserID, TaskID, AssignedAt)
VALUES
    (2, 2, @task_a, NOW()),
    (2, 2, @task_b, NOW() + INTERVAL 1 MINUTE);

-- Step 3: make sure Rule 1 does not block this test
INSERT INTO EnergyStatus (ChargingSessionID, BatteryLevel, RobotID, RecordedAt)
VALUES
    (2, 80, 2, NOW() + INTERVAL 10 MINUTE);

-- Step 4: first active task for robot 2
-- SHOULD PASS
INSERT INTO TaskExecutions (TaskID, StartTime, EndTime, Status)
VALUES
    (@task_a, NOW(), NOW() + INTERVAL 30 MINUTE, 0);

-- Step 5: second active task for the same robot
-- SHOULD FAIL
INSERT INTO TaskExecutions (TaskID, StartTime, EndTime, Status)
VALUES
    (@task_b, NOW() + INTERVAL 1 MINUTE, NOW() + INTERVAL 40 MINUTE, 0);