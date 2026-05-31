"""Domain-level validation helpers for Robo-Nexus."""

from datetime import datetime


ALERT_SEVERITIES = {"Low", "Medium", "High", "Critical"}
TASK_PRIORITIES = {"1", "2", "3", "4", "5"}
ENVIRONMENT_TYPES = {"HOME", "HOSPITAL", "WAREHOUSE", "FACTORY"}


def require_non_empty(value: str, field_name: str) -> str:
    """Ensure a string field is present and not blank."""
    if value is None or not str(value).strip():
        raise ValueError(f"{field_name} is required.")
    return str(value).strip()


def require_positive_int(value: int, field_name: str) -> int:
    """Ensure an integer id/value is positive."""
    if not isinstance(value, int) or value <= 0:
        raise ValueError(f"{field_name} must be a positive integer.")
    return value


def require_in_set(value: str, allowed_values: set[str], field_name: str) -> str:
    """Ensure value belongs to a predefined domain."""
    if value not in allowed_values:
        allowed_display = ", ".join(sorted(allowed_values))
        raise ValueError(f"{field_name} must be one of: {allowed_display}.")
    return value


def validate_task_priority(priority: str) -> str:
    """Validate task priority domain."""
    return require_in_set(priority, TASK_PRIORITIES, "priority")


def validate_alert_severity(severity: str) -> str:
    """Validate alert severity domain."""
    return require_in_set(severity, ALERT_SEVERITIES, "severity")


def validate_environment_type(environment_type: str) -> str:
    """Validate deployment environment type domain."""
    return require_in_set(environment_type, ENVIRONMENT_TYPES, "environment_type")


def validate_time_range(start_time: datetime, end_time: datetime) -> None:
    """Ensure end time is not earlier than start time."""
    if end_time < start_time:
        raise ValueError("end_time cannot be earlier than start_time.")

