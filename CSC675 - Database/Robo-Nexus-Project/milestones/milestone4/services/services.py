"""Public service-layer API for the Robo-Nexus database.

Service methods expose business operations and rely on ORM model methods.
No raw SQL should appear in this file.
"""

from datetime import datetime
from typing import Any, List, Optional

from models.models import (
    AIModels,
    AIRequests,
    AIResponses,
    Robots,
    SupportRequests,
    TaskAssignments,
    Tasks,
    Users,
)


def create_user(user_name: str) -> Any:
    """Create and persist a user record."""
    if not user_name or not user_name.strip():
        raise ValueError("user_name is required.")

    user = Users(UserName=user_name.strip())
    user.save()
    return user


def register_robot(
    robot_role_id: int,
    robot_model_id: int,
    user_id: int,
    software_version_id: int,
    deployment_location_id: Optional[int] = None,
) -> Any:
    """Register a robot in the fleet."""
    robot = Robots(
        RobotRoleID=robot_role_id,
        RobotModelID=robot_model_id,
        UserID=user_id,
        SoftwareVersionID=software_version_id,
        DeploymentLocationID=deployment_location_id,
    )
    robot.save()
    return robot


def create_task(task_name: str, description: str, priority: str, status: int = 0) -> Any:
    """Create a task that can later be assigned to a robot."""
    if not task_name or not task_name.strip():
        raise ValueError("task_name is required.")
    if priority not in {"1", "2", "3", "4", "5"}:
        raise ValueError("priority must be one of: 1, 2, 3, 4, 5.")

    task = Tasks(
        TaskName=task_name.strip(),
        Description=description.strip(),
        Priority=priority,
        Status=status,
    )
    task.save()
    return task


def assign_task_to_robot(robot_id: int, user_id: int, task_id: int) -> Any:
    """Assign an existing task to a robot."""
    assignment = TaskAssignments(
        RobotID=robot_id,
        UserID=user_id,
        TaskID=task_id,
        AssignedAt=datetime.now(),
    )
    assignment.save()
    return assignment


def create_support_request(robot_id: int, issue_details: str) -> Any:
    """Open a support request for a robot issue."""
    if not issue_details or not issue_details.strip():
        raise ValueError("issue_details is required.")

    support_request = SupportRequests(
        RobotID=robot_id,
        IssueDetails=issue_details.strip(),
        TimeReported=datetime.now(),
    )
    support_request.save()
    return support_request


def get_robot_by_id(robot_id: int) -> Optional[Any]:
    """Fetch one robot by primary key."""
    if robot_id <= 0:
        raise ValueError("robot_id must be a positive integer.")
    return Robots.get(RobotID=robot_id)


def list_robot_tasks(robot_id: int) -> List[Any]:
    """List task assignments for a robot."""
    if robot_id <= 0:
        raise ValueError("robot_id must be a positive integer.")
    return TaskAssignments.filter(RobotID=robot_id)


def register_ai_model(model_name: str, service_type: str, version_label: str) -> Any:
    """Create and persist an AI model record."""
    if not model_name or not model_name.strip():
        raise ValueError("model_name is required.")
    if not service_type or not service_type.strip():
        raise ValueError("service_type is required.")
    if not version_label or not version_label.strip():
        raise ValueError("version_label is required.")

    ai_model = AIModels(
        ModelName=model_name.strip(),
        ServiceType=service_type.strip(),
        VersionLabel=version_label.strip(),
    )
    ai_model.save()
    return ai_model


def create_ai_request(ai_model_id: int, robot_id: int) -> Any:
    """Create an AI request for a robot using a model."""
    if ai_model_id <= 0:
        raise ValueError("ai_model_id must be a positive integer.")
    if robot_id <= 0:
        raise ValueError("robot_id must be a positive integer.")

    ai_request = AIRequests(
        AIModelID=ai_model_id,
        RobotID=robot_id,
        RequestedAt=datetime.now(),
    )
    ai_request.save()
    return ai_request


def create_ai_response(
    ai_request_id: int,
    robot_role_id: int,
    task_id: int,
    response_text: str,
) -> Any:
    """Create an AI response linked to request, role, and task."""
    if ai_request_id <= 0:
        raise ValueError("ai_request_id must be a positive integer.")
    if robot_role_id <= 0:
        raise ValueError("robot_role_id must be a positive integer.")
    if task_id <= 0:
        raise ValueError("task_id must be a positive integer.")
    if not response_text or not response_text.strip():
        raise ValueError("response_text is required.")

    ai_response = AIResponses(
        AIRequestsID=ai_request_id,
        RobotRoleID=robot_role_id,
        TaskID=task_id,
        GeneratedAt=datetime.now(),
        Response=response_text.strip(),
    )
    ai_response.save()
    return ai_response

