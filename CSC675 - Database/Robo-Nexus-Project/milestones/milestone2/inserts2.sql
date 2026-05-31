USE robo_nexus;

-- =====================================================
-- ADDITIONAL DATA SET #2 / #3 / #4
-- Assumes your current inserts.sql has already created ID=1 rows
-- =====================================================

-- =========================
-- Manufacturers
-- =========================
INSERT INTO Manufacturers (Name)
VALUES
    ('AutoMech'),
    ('CareBots'),
    ('FactoryMind');

-- =========================
-- Users
-- =========================
INSERT INTO Users (UserName)
VALUES
    ('Bob'),
    ('Carol'),
    ('David');

-- =========================
-- Addresses
-- =========================
INSERT INTO Addresses (Street, City, State, Zipcode)
VALUES
    ('456 Oak Ave', 'San Jose', 'CA', '95112'),
    ('789 Pine Rd', 'Oakland', 'CA', '94607'),
    ('1010 Market St', 'Sacramento', 'CA', '95814');

-- =========================
-- Deployment Locations
-- =========================
INSERT INTO DeploymentLocations (EnvironmentType, AddressID)
VALUES
    ('HOME', 2),
    ('WAREHOUSE', 3),
    ('FACTORY', 4);

-- =========================
-- Software Versions
-- =========================
INSERT INTO SoftwareVersions ()
VALUES (), (), ();

-- =========================
-- AI Models
-- =========================
INSERT INTO AIModels (ModelName, ServiceType, VersionLabel)
VALUES
    ('TaskPlanner', 'Planning', 'v2'),
    ('HealthMonitor', 'Diagnostic', 'v1'),
    ('WarehouseOptimizer', 'Logistics', 'v3');

-- =========================
-- Tasks
-- Insert tasks before AIResponses so RobotRoles can reference future response ids
-- =========================
INSERT INTO Tasks (TaskName, Description, Priority, Status)
VALUES
    ('Check Patient Vitals', 'Collect patient vital signs in room 202', '2', 0),
    ('Move Inventory', 'Transport inventory from shelf A to dock B', '4', 0),
    ('Inspect Assembly Line', 'Run visual inspection on line 3', '3', 0);

-- =========================
-- Robot Roles
-- Note: schema includes AIResponseID column, but no FK is defined on it
-- so it is safe to reference the ids that will exist after AIResponses insert.
-- Existing row was RobotRoleID=1.
-- =========================
INSERT INTO RobotRoles (RobotRole, UserID, AIResponseID)
VALUES
    ('Worker', 2, 2),
    ('Senior', 3, 3),
    ('Worker', 4, 4);

-- =========================
-- Robot Models
-- =========================
INSERT INTO RobotModels (Model, ManufacturerID)
VALUES
    ('Model2', 2),
    ('Model1', 3),
    ('Model2', 4);

-- =========================
-- Robots
-- =========================
INSERT INTO Robots (RobotRoleID, RobotModelID, DeploymentLocationID, UserID, SoftwareVersionID)
VALUES
    (2, 2, 2, 2, 2),
    (3, 3, 3, 3, 3),
    (4, 4, 4, 4, 4);

-- =========================
-- Charging Stations
-- =========================
INSERT INTO ChargingStations (StationName, Capacity, Status)
VALUES
    ('Station-B', 2, 'AVAILABLE'),
    ('Station-C', 3, 'AVAILABLE'),
    ('Station-D', 1, 'MAINTENANCE');

-- =========================
-- Charging Sessions
-- Session 2 and 3 are active (EndTime NULL), session 4 ended
-- =========================
INSERT INTO ChargingSessions (DeploymentLocationID, StartTime, EndTime)
VALUES
    (2, NOW() - INTERVAL 2 HOUR, NULL),
    (3, NOW() - INTERVAL 90 MINUTE, NULL),
    (4, NOW() - INTERVAL 4 HOUR, NOW() - INTERVAL 3 HOUR);

-- =========================
-- Energy Status
-- =========================
INSERT INTO EnergyStatus (ChargingSessionID, BatteryLevel, RobotID)
VALUES
    (2, 82, 2),
    (3, 61, 3),
    (4, 28, 4);

-- =========================
-- AI Requests
-- =========================
INSERT INTO AIRequests (AIModelID, RobotID, RequestedAt)
VALUES
    (2, 2, NOW() - INTERVAL 50 MINUTE),
    (3, 3, NOW() - INTERVAL 40 MINUTE),
    (4, 4, NOW() - INTERVAL 30 MINUTE);

-- =========================
-- AI Responses
-- =========================
INSERT INTO AIResponses (AIRequestsID, RobotRoleID, TaskID, GeneratedAt, Response)
VALUES
    (2, 2, 2, NOW() - INTERVAL 49 MINUTE, 'Recommended task: Check Patient Vitals'),
    (3, 3, 3, NOW() - INTERVAL 39 MINUTE, 'Recommended task: Move Inventory'),
    (4, 4, 4, NOW() - INTERVAL 29 MINUTE, 'Recommended task: Inspect Assembly Line');

-- =========================
-- Task Assignments
-- =========================
INSERT INTO TaskAssignments (RobotID, UserID, TaskID, AssignedAt)
VALUES
    (2, 2, 2, NOW() - INTERVAL 45 MINUTE),
    (3, 3, 3, NOW() - INTERVAL 35 MINUTE),
    (4, 4, 4, NOW() - INTERVAL 25 MINUTE);

-- =========================
-- Task Executions
-- Status kept numeric to match your original script
-- =========================
INSERT INTO TaskExecutions (TaskID, StartTime, EndTime, Status)
VALUES
    (2, NOW() - INTERVAL 44 MINUTE, NOW() - INTERVAL 20 MINUTE, 1),
    (3, NOW() - INTERVAL 34 MINUTE, NOW() - INTERVAL 10 MINUTE, 1),
    (4, NOW() - INTERVAL 24 MINUTE, NOW() - INTERVAL 5 MINUTE, 1);

-- =========================
-- Alerts
-- =========================
INSERT INTO Alerts (RobotID, Severity, TimeStamp)
VALUES
    (2, 'Medium', NOW() - INTERVAL 30 MINUTE),
    (3, 'Low', NOW() - INTERVAL 25 MINUTE),
    (4, 'High', NOW() - INTERVAL 15 MINUTE);

-- =========================
-- Performance Metrics
-- =========================
INSERT INTO PerformanceMetrics (RobotID, MetricType, MetricValue, TimeStamp)
VALUES
    (2, 'BatteryEfficiency', 91, NOW() - INTERVAL 30 MINUTE),
    (3, 'LoadCapacity', 77, NOW() - INTERVAL 20 MINUTE),
    (4, 'Accuracy', 84, NOW() - INTERVAL 10 MINUTE);

-- =========================
-- Technicians
-- =========================
INSERT INTO Technicians (DeploymentLocationID)
VALUES
    (2),
    (3),
    (4);

-- =========================
-- Technician Names
-- =========================
INSERT INTO TechnicianNames (TechnicianID, FirstName, LastName)
VALUES
    (2, 'Mia', 'Chen'),
    (3, 'Noah', 'Patel'),
    (4, 'Emma', 'Lopez');

-- =========================
-- Maintenance Record
-- note schema uses TechinicianID
-- =========================
INSERT INTO MaintenanceRecord (RobotID, TechinicianID, RecordedAt, Description)
VALUES
    (2, 2, NOW() - INTERVAL 12 HOUR, 'Wheel calibration and battery check'),
    (3, 3, NOW() - INTERVAL 10 HOUR, 'Forklift arm inspection completed'),
    (4, 4, NOW() - INTERVAL 8 HOUR, 'Camera alignment and firmware check');

-- =========================
-- Hardwares
-- =========================
INSERT INTO Hardwares (ManufacturerID)
VALUES
    (2),
    (3),
    (4);

-- =========================
-- RobotHardware
-- =========================
INSERT INTO RobotHardware (RobotID)
VALUES
    (2),
    (3),
    (4);

-- =========================
-- Logs
-- =========================
INSERT INTO Logs (RobotID, EventType, EventDetails)
VALUES
    (2, 'PATIENT_CHECK', 'Vitals captured for patient in room 202'),
    (3, 'INVENTORY_MOVE', 'Moved pallet from A1 to Dock B'),
    (4, 'INSPECTION', 'Assembly line visual inspection completed');

-- =========================
-- Sensors
-- =========================
INSERT INTO Sensors (RobotID, SensorType)
VALUES
    (2, 'HeartRate'),
    (3, 'Weight'),
    (4, 'Vision');

-- =========================
-- Emotional Patterns
-- =========================
INSERT INTO EmotionalPatterns (RobotID, AIModelID, PatternSummary)
VALUES
    (2, 2, 'Supportive interactions with patients'),
    (3, 3, 'Neutral warehouse interactions'),
    (4, 4, 'Focused operational behavior');

-- =========================
-- Emotional Records
-- note schema column is Emotional_ID, EmotionalState ENUM("1","2","3")
-- =========================
INSERT INTO EmotionalRecords (EmotionalPatternID, EmotionalState, Timestamp, Description)
VALUES
    (2, '2', NOW() - INTERVAL 2 HOUR, 'Patient appeared reassured'),
    (3, '1', NOW() - INTERVAL 90 MINUTE, 'Routine warehouse interaction'),
    (4, '3', NOW() - INTERVAL 70 MINUTE, 'Detected elevated operator stress');

-- =========================
-- Homes / Hospitals / Warehouses / Factories
-- =========================
INSERT INTO Homes (DeploymentLocationID)
VALUES (2);

INSERT INTO Warehouses (DeploymentLocationID)
VALUES (3);

INSERT INTO Factories (DeploymentLocationID)
VALUES (4);

INSERT INTO Hospitals (DeploymentLocationID)
VALUES (2);

-- =========================
-- Departments
-- note schema uses HosptialID
-- =========================
INSERT INTO Department (DepartmentName, HosptialID)
VALUES
    ('Emergency', 2),
    ('Logistics Support', 1);

-- =========================
-- Interaction Sessions
-- note schema spells StartTIme / EndTIme
-- use the actual column names from schema if needed
-- =========================
INSERT INTO InteractionSessions (RobotID, UserID, HomeID, StartTIme, EndTIme)
VALUES
    (2, 2, 2, NOW() - INTERVAL 3 HOUR, NOW() - INTERVAL 170 MINUTE);

-- =========================
-- Diagnostic Reports
-- =========================
INSERT INTO DiagnosticReports (RobotID, AIModelID, GeneratedAt, Summary)
VALUES
    (2, 2, NOW() - INTERVAL 1 HOUR, 'All patient-support subsystems normal'),
    (3, 3, NOW() - INTERVAL 50 MINUTE, 'Load sensor variance within range'),
    (4, 4, NOW() - INTERVAL 40 MINUTE, 'Optical inspection model performing normally');

-- =========================
-- Support Requests
-- =========================
INSERT INTO SupportRequests (RobotID, IssueDetails, TimeReported)
VALUES
    (2, 'Sensor drift warning after vitals capture', NOW() - INTERVAL 55 MINUTE),
    (3, 'Drive wheel vibration detected during transport', NOW() - INTERVAL 45 MINUTE),
    (4, 'Camera focus needs recalibration', NOW() - INTERVAL 35 MINUTE);

-- =========================
-- Associative Tables
-- =========================
INSERT INTO UserDeploymentLocation (DeploymentLocationID, UserID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO ChargingStationSessions (ChargingSessionID, ChargingStationID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO RobotChargingStations (ChargingStationID, RobotID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO LogSessions (ChargingSessionID, LogID)
VALUES
    (2, 3),
    (3, 4),
    (4, 5);

INSERT INTO EmotionalAnalyzes (EmotionalID, AIRequestID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO PerformanceDiagnostics (DiagnosticReportID, PerformanceMetricID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO EmotionalInteractionSessions (EmotionalID, InteractionID)
VALUES
    (2, 2);

INSERT INTO TechnicianSupportRequests (SupportRequestID, TechnicianID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO HardwareMaintenances (HardwareID, MaintenanceRecordID)
VALUES
    (2, 2),
    (3, 3),
    (4, 4);

INSERT INTO PatientEmotionalRecords (EmotionalID, HospitalID)
VALUES
    (2, 2),
    (3, 1),
    (4, 2);

-- =========================
-- timestamps helper table
-- =========================
INSERT INTO timestamps (update_time)
VALUES
    (NOW()),
    (NOW()),
    (NOW());