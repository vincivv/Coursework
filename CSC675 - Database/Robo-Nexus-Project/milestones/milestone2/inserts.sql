USE robo_nexus;

-- =========================
-- Manufacturers
-- =========================
INSERT INTO Manufacturers (Name)
VALUES ('RoboCorp');

-- =========================
-- Users
-- =========================
INSERT INTO Users (UserName)
VALUES ('Alice');

-- =========================
-- Addresses
-- =========================
INSERT INTO Addresses (Street, City, State, Zipcode)
VALUES ('123 Main St', 'San Francisco', 'CA', '94132');

-- =========================
-- Deployment Locations
-- =========================
INSERT INTO DeploymentLocations (EnvironmentType, AddressID)
VALUES ('HOSPITAL', 1);

-- =========================
-- Software Versions
-- =========================
INSERT INTO SoftwareVersions ()
VALUES ();

-- =========================
-- AI Models
-- =========================
INSERT INTO AIModels (ModelName, ServiceType, VersionLabel)
VALUES ('EmotionAnalyzer', 'Emotion', 'v1');

-- =========================
-- Robot Models
-- =========================
INSERT INTO RobotModels (Model, ManufacturerID)
VALUES ('Model1', 1);

-- =========================
-- Robot Roles
-- =========================
INSERT INTO RobotRoles (RobotRole, UserID,AIResponseID)
VALUES ('Senior', 1,1);

-- =========================
-- Robots
-- =========================
INSERT INTO Robots (RobotRoleID, RobotModelID, DeploymentLocationID, UserID, SoftwareVersionID)
VALUES (1, 1, 1, 1, 1);

-- =========================
-- Charging Stations
-- =========================
INSERT INTO ChargingStations (StationName, Capacity)
VALUES ('Station-A', 5);

-- =========================
-- Charging Sessions
-- =========================
INSERT INTO ChargingSessions (DeploymentLocationID, StartTime, EndTime)
VALUES (1, NOW(), NOW());

-- =========================
-- Energy Status
-- =========================
INSERT INTO EnergyStatus (ChargingSessionID, BatteryLevel, RobotID)
VALUES (1, 95, 1);

-- =========================
-- Tasks
-- =========================
INSERT INTO Tasks (TaskName, Description, Priority, Status)
VALUES ('Deliver Medicine', 'Deliver medication to room 101', '3', 0);

-- =========================
-- AI Requests
-- =========================
INSERT INTO AIRequests (AIModelID, RobotID, RequestedAt)
VALUES (1, 1, NOW());

-- =========================
-- AI Responses
-- =========================
INSERT INTO AIResponses (AIRequestsID, RobotRoleID, TaskID, GeneratedAt, Response)
VALUES (1, 1, 1, NOW(), 'Recommended task: Deliver Medicine');

-- =========================
-- Task Assignments
-- =========================
INSERT INTO TaskAssignments (RobotID, UserID, TaskID, AssignedAt)
VALUES (1, 1, 1, NOW());

-- =========================
-- Task Executions
-- =========================
INSERT INTO TaskExecutions (TaskID, StartTime, EndTime, Status)
VALUES (1, NOW(), NOW(), 1);

-- =========================
-- Alerts
-- =========================
INSERT INTO Alerts (RobotID, Severity, TimeStamp)
VALUES (1, 'Low', NOW());

-- =========================
-- Performance Metrics
-- =========================
INSERT INTO PerformanceMetrics (RobotID, MetricType, MetricValue, TimeStamp)
VALUES (1, 'Speed', 88, NOW());

-- =========================
-- Technicians
-- =========================
INSERT INTO Technicians (DeploymentLocationID)
VALUES (1);

-- =========================
-- Maintenance Record
-- =========================
INSERT INTO MaintenanceRecord (RobotID, TechinicianID, RecordedAt, Description)
VALUES (1, 1, NOW(), 'Routine maintenance completed');

-- =========================
-- Hardwares
-- =========================
INSERT INTO Hardwares (ManufacturerID)
VALUES (1);

-- =========================
-- RobotHardware
-- =========================
INSERT INTO RobotHardware (RobotID)
VALUES (1);

-- =========================
-- Logs
-- =========================
INSERT INTO Logs (RobotID, EventType, EventDetails)
VALUES
(1, 'SYSTEM_START', 'Robot boot sequence completed'),
(1, 'TASK_COMPLETED', 'Task executed successfully');

-- =========================
-- Sensors
-- =========================
INSERT INTO Sensors (RobotID, SensorType)
VALUES (1, 'Temperature');

-- =========================
-- Emotional Patterns
-- =========================
INSERT INTO EmotionalPatterns (RobotID, AIModelID, PatternSummary)
VALUES (1, 1, 'Mostly calm interactions');

-- =========================
-- Emotional Records
-- =========================
INSERT INTO EmotionalRecords (EmotionalPatternID, EmotionalState, Timestamp, Description)
VALUES (1, '1', NOW(), 'Calm interaction detected');

-- =========================
-- Homes
-- =========================
INSERT INTO Homes (DeploymentLocationID)
VALUES (1);

-- =========================
-- Hospitals
-- =========================
INSERT INTO Hospitals (DeploymentLocationID)
VALUES (1);

-- =========================
-- Warehouses
-- =========================
INSERT INTO Warehouses (DeploymentLocationID)
VALUES (1);

-- =========================
-- Factories
-- =========================
INSERT INTO Factories (DeploymentLocationID)
VALUES (1);

-- =========================
-- Departments
-- =========================
INSERT INTO Department (DepartmentName, HosptialID)
VALUES ('Cardiology', 1);

-- =========================
-- Interaction Sessions
-- =========================
INSERT INTO InteractionSessions (RobotID, UserID, HomeID, StartTime, EndTime)
VALUES (1, 1, 1, NOW(), NOW());

-- =========================
-- Diagnostic Reports
-- =========================
INSERT INTO DiagnosticReports (RobotID, AIModelID, GeneratedAt, Summary)
VALUES (1, 1, NOW(), 'System normal');

-- =========================
-- Support Requests
-- =========================
INSERT INTO SupportRequests (RobotID, IssueDetails, TimeReported)
VALUES (1, 'Low battery warning', NOW());

-- =========================
-- Associative Tables
-- =========================

INSERT INTO UserDeploymentLocation (DeploymentLocationID, UserID)
VALUES (1, 1);

INSERT INTO ChargingStationSessions (ChargingSessionID, ChargingStationID)
VALUES (1, 1);

INSERT INTO RobotChargingStations (ChargingStationID, RobotID)
VALUES (1, 1);

INSERT INTO LogSessions (ChargingSessionID, LogID)
VALUES (1, 1);

INSERT INTO EmotionalAnalyzes (EmotionalID, AIRequestID)
VALUES (1, 1);

INSERT INTO PerformanceDiagnostics (DiagnosticReportID, PerformanceMetricID)
VALUES (1, 1);

INSERT INTO EmotionalInteractionSessions (EmotionalID, InteractionID)
VALUES (1, 1);

INSERT INTO TechnicianSupportRequests (SupportRequestID, TechnicianID)
VALUES (1, 1);

INSERT INTO HardwareMaintenances (HardwareID, MaintenanceRecordID)
VALUES (1, 1);

INSERT INTO PatientEmotionalRecords (EmotionalID, HospitalID)
VALUES (1, 1);