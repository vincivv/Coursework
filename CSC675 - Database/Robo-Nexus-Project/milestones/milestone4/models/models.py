from orm.base import Base
from orm.columns import Column
from orm.datatypes import Integer, String, DateTime
from orm.relationships import Relationship


class User(Base):
    __tablename__ = "Users"

    UserID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    UserName = Column(String(length=45), nullable=False)

    def __repr__(self):
        return f"Users(UserID={self.UserID}, UserName={self.UserName})"


class RobotRole(Base):
    __tablename__ = "RobotRoles"

    RobotRoleID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    RobotRole = Column(String(type="ENUM('Senior','Worker')"), nullable=True)
    UserID = Column(Integer, nullable=False, foreign_key="Users(UserID)")
    AIResponseID = Column(Integer, nullable=False, foreign_key="AIResponses(AIResponseID)")

    user = Relationship("Users", "UserID", backreference="robot_roles", lazy_load=True)
    ai_response = Relationship("AIResponses", "AIResponseID", backreference="robot_roles", lazy_load=True)

    def __repr__(self):
        return f"RobotRoles(RobotRoleID={self.RobotRoleID}, RobotRole={self.RobotRole})"


class RobotModel(Base):
    __tablename__ = "RobotModels"

    RobotModelID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    Model = Column(String(type="ENUM('Model1','Model2')"), nullable=True)
    ManufacturerID = Column(Integer, nullable=True)

    def __repr__(self):
        return f"RobotModels(RobotModelID={self.RobotModelID}, Model={self.Model})"


class DeploymentLocation(Base):
    __tablename__ = "DeploymentLocations"

    DeploymentLocationID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    EnvironmentType = Column(
        String(type="ENUM('HOME','HOSPITAL','WAREHOUSE','FACTORY')"),
        nullable=False
    )
    AddressID = Column(Integer, nullable=False)

    def __repr__(self):
        return f"DeploymentLocations(ID={self.DeploymentLocationID})"


class SoftwareVersion(Base):
    __tablename__ = "SoftwareVersions"

    SoftwareVersionID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)

    def __repr__(self):
        return f"SoftwareVersions(ID={self.SoftwareVersionID})"


class Robot(Base):
    __tablename__ = "Robots"

    RobotID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    RobotRoleID = Column(Integer, nullable=False, foreign_key="RobotRoles(RobotRoleID)")
    RobotModelID = Column(Integer, nullable=False, foreign_key="RobotModels(RobotModelID)")
    DeploymentLocationID = Column(Integer, nullable=True, foreign_key="DeploymentLocations(DeploymentLocationID)")
    UserID = Column(Integer, nullable=False, foreign_key="Users(UserID)")
    SoftwareVersionID = Column(Integer, nullable=False, foreign_key="SoftwareVersions(SoftwareVersionID)")

    robot_role = Relationship("RobotRoles", "RobotRoleID", backreference="robots", lazy_load=True)
    robot_model = Relationship("RobotModels", "RobotModelID", backreference="robots", lazy_load=True)
    deployment_location = Relationship("DeploymentLocations", "DeploymentLocationID", backreference="robots", lazy_load=True)
    owner_user = Relationship("Users", "UserID", backreference="robots", lazy_load=True)
    software_version = Relationship("SoftwareVersions", "SoftwareVersionID", backreference="robots", lazy_load=True)

    @property
    def user(self):
        """Alias for owner_user (Users row for this robot's UserID)."""
        return self.owner_user

    def __repr__(self):
        return f"Robots(RobotID={self.RobotID})"


class Task(Base):
    __tablename__ = "Tasks"

    TaskID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    TaskName = Column(String(length=64), nullable=False)
    Description = Column(String(type="TEXT"), nullable=False)
    Priority = Column(String(type="ENUM('1','2','3','4','5')"), nullable=False)
    Status = Column(Integer(type="TINYINT"), nullable=False)

    def __repr__(self):
        return f"Tasks(TaskID={self.TaskID})"


class TaskAssignment(Base):
    __tablename__ = "TaskAssignments"

    TaskAssignmentID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    RobotID = Column(Integer, nullable=False, foreign_key="Robots(RobotID)")
    UserID = Column(Integer, nullable=False, foreign_key="Users(UserID)")
    TaskID = Column(Integer, nullable=False, foreign_key="Tasks(TaskID)")
    AssignedAt = Column(DateTime(type="TIMESTAMP"), nullable=False)

    robot = Relationship("Robots", "RobotID", backreference="task_assignments", lazy_load=True)
    user = Relationship("Users", "UserID", backreference="task_assignments", lazy_load=True)
    task = Relationship("Tasks", "TaskID", backreference="task_assignments", lazy_load=True)

    def __repr__(self):
        return f"TaskAssignments(TaskAssignmentID={self.TaskAssignmentID})"


class SupportRequest(Base):
    __tablename__ = "SupportRequests"

    SupportRequestID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    RobotID = Column(Integer, nullable=False, foreign_key="Robots(RobotID)")
    IssueDetails = Column(String(type="TEXT"), nullable=False)
    TimeReported = Column(DateTime, nullable=False)

    robot = Relationship("Robots", "RobotID", backreference="support_requests", lazy_load=True)

    def __repr__(self):
        return f"SupportRequests(SupportRequestID={self.SupportRequestID})"


class AIModel(Base):
    __tablename__ = "AIModels"

    AIModelID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    ModelName = Column(String(length=45), nullable=False)
    ServiceType = Column(String(length=45), nullable=False)
    VersionLabel = Column(String(length=45), nullable=False)

    def __repr__(self):
        return f"AIModels(AIModelID={self.AIModelID})"


class AIRequest(Base):
    __tablename__ = "AIRequests"

    AIRequestID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    AIModelID = Column(Integer, nullable=False, foreign_key="AIModels(AIModelID)")
    RobotID = Column(Integer, nullable=False, foreign_key="Robots(RobotID)")
    RequestedAt = Column(DateTime, nullable=False)

    ai_model = Relationship("AIModels", "AIModelID", backreference="ai_requests", lazy_load=True)
    robot = Relationship("Robots", "RobotID", backreference="ai_requests", lazy_load=True)

    def __repr__(self):
        return f"AIRequests(AIRequestID={self.AIRequestID})"


class AIResponse(Base):
    __tablename__ = "AIResponses"

    AIResponseID = Column(Integer, primary_key=True, auto_increment=True, nullable=False)
    # DB column name is AIRequestsID (see milestone2/Schema.sql); maps to AIRequests.AIRequestID
    AIRequestsID = Column(Integer, nullable=False, foreign_key="AIRequests(AIRequestID)")
    RobotRoleID = Column(Integer, nullable=False, foreign_key="RobotRoles(RobotRoleID)")
    TaskID = Column(Integer, nullable=False, foreign_key="Tasks(TaskID)")
    GeneratedAt = Column(DateTime, nullable=False)
    Response = Column(String(type="TEXT"), nullable=False)

    ai_request = Relationship("AIRequests", "AIRequestsID", backreference="ai_responses", lazy_load=True)
    robot_role = Relationship("RobotRoles", "RobotRoleID", lazy_load=True)
    task = Relationship("Tasks", "TaskID", lazy_load=True)

    def __repr__(self):
        return f"AIResponses(AIResponseID={self.AIResponseID})"