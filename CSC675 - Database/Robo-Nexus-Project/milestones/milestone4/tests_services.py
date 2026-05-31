

from models.models import (
    DeploymentLocations,
    RobotModels,
    RobotRoles,
    SoftwareVersions,
)
from services.services import (
    assign_task_to_robot,
    create_ai_request,
    create_ai_response,
    create_support_request,
    create_task,
    create_user,
    get_robot_by_id,
    list_robot_tasks,
    register_ai_model,
    register_robot,
)


def main() -> int:
    print("\n" + "=" * 70)
    print("People and site")
    print("=" * 70)

    john = create_user("John")
    michael = create_user("Michael")
    print("Accounts:", john, michael)

    deployment = DeploymentLocations.create(EnvironmentType="WAREHOUSE", AddressID=1)
    robot_model = RobotModels.create(Model="Model1", ManufacturerID=1)
    software = SoftwareVersions.create()
    role_for_fleet = RobotRoles.create(
        RobotRole="Worker",
        UserID=john.UserID,
        AIResponseID=1,
    )
    print("Warehouse site:", deployment)
    print("Robot hardware line:", robot_model)
    print("Firmware row:", software)
    print("Role record (John + existing AI response id 1):", role_for_fleet)

    print("\n" + "=" * 70)
    print("Robots on the floor")
    print("=" * 70)

    robot = register_robot(
        robot_role_id=role_for_fleet.RobotRoleID,
        robot_model_id=robot_model.RobotModelID,
        user_id=john.UserID,
        software_version_id=software.SoftwareVersionID,
        deployment_location_id=deployment.DeploymentLocationID,
    )
    robot2 = register_robot(
        robot_role_id=role_for_fleet.RobotRoleID,
        robot_model_id=robot_model.RobotModelID,
        user_id=michael.UserID,
        software_version_id=software.SoftwareVersionID,
        deployment_location_id=deployment.DeploymentLocationID,
    )
    print("John’s robot:", robot)
    print("Michael’s robot:", robot2)

    task = create_task(
        "Warehouse floor patrol",
        "Sweep aisles A–C before the night shift.",
        "2",
        status=1,
    )
    print("Work order:", task)

    a1 = assign_task_to_robot(robot.RobotID, john.UserID, task.TaskID)
    a2 = assign_task_to_robot(robot2.RobotID, michael.UserID, task.TaskID)
    print("John’s assignment:", a1)
    print("Michael’s assignment:", a2)

    sup = create_support_request(robot.RobotID, "Left drive motor running hot after hour three.")
    print("Support ticket opened:", sup)

    aim = register_ai_model(
        "Navigator-3000",
        "Path planning",
        "2025.03",
    )
    print("AI model catalog entry:", aim)

    # One open AI request per robot: respond for robot 1 before opening another request on same robot.
    req = create_ai_request(aim.AIModelID, robot.RobotID)
    print("John’s robot asked for a plan:", req)
    resp = create_ai_response(
        ai_request_id=req.AIRequestID,
        robot_role_id=role_for_fleet.RobotRoleID,
        task_id=task.TaskID,
        response_text="Route approved; avoid aisle B east end (spill cleanup).",
    )
    print("Planner reply to John’s robot:", resp)

    req2 = create_ai_request(aim.AIModelID, robot2.RobotID)
    print("Michael’s robot asked for a plan:", req2)
    resp2 = create_ai_response(
        ai_request_id=req2.AIRequestID,
        robot_role_id=role_for_fleet.RobotRoleID,
        task_id=task.TaskID,
        response_text="Same route family; start at bay 12.",
    )
    print("Planner reply to Michael’s robot:", resp2)

    print("\n" + "=" * 70)
    print("Lookups")
    print("=" * 70)

    fetched = get_robot_by_id(robot.RobotID)
    print("Fetch John’s robot by id:", fetched)

    tasks_for_r1 = list_robot_tasks(robot.RobotID)
    print("Tasks currently on John’s robot:", tasks_for_r1)

    print("\n" + "=" * 70)
    print("Bad input")
    print("=" * 70)
    try:
        create_user("   ")
    except ValueError as e:
        print("Rejected blank name:", e)

    print("\n" + "=" * 70)
    print("Done.")
    print("=" * 70)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
