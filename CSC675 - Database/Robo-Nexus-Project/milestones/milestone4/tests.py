


from datetime import datetime

from models.models import (
    AIModels,
    AIRequests,
    AIResponses,
    DeploymentLocations,
    RobotModels,
    RobotRoles,
    Robots,
    SoftwareVersions,
    SupportRequests,
    TaskAssignments,
    Tasks,
    Users,
)


def main() -> int:
    print("\n" + "=" * 70)
    print("SETUP — users, deployments, models, software, robots, tasks, roles, AI, assignments")
    print("=" * 70)

    john = Users.create(UserName="John")
    michael = Users.create(UserName="Michael")
    print("User 1 (John):", john)
    print("User 2 (Michael):", michael)

    deployment_1 = DeploymentLocations.create(EnvironmentType="HOME", AddressID=1)
    deployment_2 = DeploymentLocations.create(EnvironmentType="HOSPITAL", AddressID=1)
    print("Deployment 1:", deployment_1)
    print("Deployment 2:", deployment_2)

    robot_model_1 = RobotModels.create(Model="Model1", ManufacturerID=1)
    robot_model_2 = RobotModels.create(Model="Model2", ManufacturerID=1)
    print("Robot model 1:", robot_model_1)
    print("Robot model 2:", robot_model_2)

    # ORM SoftwareVersions has no version string; first row = logical 1.1, second = 1.2
    software_1_1 = SoftwareVersions.create()
    software_1_2 = SoftwareVersions.create()
    print("Software version (logical 1.1):", software_1_1)
    print("Software version (logical 1.2):", software_1_2)

    role = RobotRoles.create(
        RobotRole="Worker",
        UserID=john.UserID,
        AIResponseID=1,
    )
    print("RobotRoles (shared by all robots, AIResponseID=1):", role)

    # Robot 1–2 → deployment 1; robot 3–4 → deployment 2. Alternate models & software.
    robot_1 = Robots.create(
        RobotRoleID=role.RobotRoleID,
        RobotModelID=robot_model_1.RobotModelID,
        DeploymentLocationID=deployment_1.DeploymentLocationID,
        UserID=john.UserID,
        SoftwareVersionID=software_1_1.SoftwareVersionID,
    )
    robot_2 = Robots.create(
        RobotRoleID=role.RobotRoleID,
        RobotModelID=robot_model_2.RobotModelID,
        DeploymentLocationID=deployment_1.DeploymentLocationID,
        UserID=john.UserID,
        SoftwareVersionID=software_1_2.SoftwareVersionID,
    )
    robot_3 = Robots.create(
        RobotRoleID=role.RobotRoleID,
        RobotModelID=robot_model_1.RobotModelID,
        DeploymentLocationID=deployment_2.DeploymentLocationID,
        UserID=michael.UserID,
        SoftwareVersionID=software_1_1.SoftwareVersionID,
    )
    robot_4 = Robots.create(
        RobotRoleID=role.RobotRoleID,
        RobotModelID=robot_model_2.RobotModelID,
        DeploymentLocationID=deployment_2.DeploymentLocationID,
        UserID=michael.UserID,
        SoftwareVersionID=software_1_2.SoftwareVersionID,
    )
    print("Robot 1 (dep1, John, model1, sw1.1):", robot_1)
    print("Robot 2 (dep1, John, model2, sw1.2):", robot_2)
    print("Robot 3 (dep2, Michael, model1, sw1.1):", robot_3)
    print("Robot 4 (dep2, Michael, model2, sw1.2):", robot_4)

    task_a = Tasks.create(
        TaskName="Task-A",
        Description="Work order A",
        Priority="2",
        Status=1,
    )
    task_b = Tasks.create(
        TaskName="Task-B",
        Description="Work order B",
        Priority="3",
        Status=1,
    )
    print("Tasks:", task_a, task_b)

    ai_model_1 = AIModels.create(
        ModelName="AI Model 1",
        ServiceType="Planning",
        VersionLabel="1.0",
    )
    ai_model_2 = AIModels.create(
        ModelName="AI Model 2",
        ServiceType="Vision",
        VersionLabel="1.0",
    )
    print("AI model 1:", ai_model_1)
    print("AI model 2:", ai_model_2)

    now = datetime.now()
    ar1 = AIRequests.create(
        AIModelID=ai_model_1.AIModelID,
        RobotID=robot_1.RobotID,
        RequestedAt=now,
    )
    ar2 = AIRequests.create(
        AIModelID=ai_model_2.AIModelID,
        RobotID=robot_2.RobotID,
        RequestedAt=now,
    )
    ar3 = AIRequests.create(
        AIModelID=ai_model_1.AIModelID,
        RobotID=robot_3.RobotID,
        RequestedAt=now,
    )
    ar4 = AIRequests.create(
        AIModelID=ai_model_2.AIModelID,
        RobotID=robot_4.RobotID,
        RequestedAt=now,
    )
    print("AIRequests (one per robot):", ar1, ar2, ar3, ar4)

    resp1 = AIResponses.create(
        AIRequestsID=ar1.AIRequestID,
        RobotRoleID=role.RobotRoleID,
        TaskID=task_a.TaskID,
        GeneratedAt=now,
        Response="Response for robot 1",
    )
    resp2 = AIResponses.create(
        AIRequestsID=ar2.AIRequestID,
        RobotRoleID=role.RobotRoleID,
        TaskID=task_a.TaskID,
        GeneratedAt=now,
        Response="Response for robot 2",
    )
    resp3 = AIResponses.create(
        AIRequestsID=ar3.AIRequestID,
        RobotRoleID=role.RobotRoleID,
        TaskID=task_b.TaskID,
        GeneratedAt=now,
        Response="Response for robot 3",
    )
    resp4 = AIResponses.create(
        AIRequestsID=ar4.AIRequestID,
        RobotRoleID=role.RobotRoleID,
        TaskID=task_b.TaskID,
        GeneratedAt=now,
        Response="Response for robot 4",
    )
    print("AIResponses:", resp1, resp2, resp3, resp4)

    ta1 = TaskAssignments.create(
        RobotID=robot_1.RobotID,
        UserID=john.UserID,
        TaskID=task_a.TaskID,
        AssignedAt=now,
    )
    ta2 = TaskAssignments.create(
        RobotID=robot_2.RobotID,
        UserID=john.UserID,
        TaskID=task_b.TaskID,
        AssignedAt=now,
    )
    ta3 = TaskAssignments.create(
        RobotID=robot_3.RobotID,
        UserID=michael.UserID,
        TaskID=task_a.TaskID,
        AssignedAt=now,
    )
    ta4 = TaskAssignments.create(
        RobotID=robot_4.RobotID,
        UserID=michael.UserID,
        TaskID=task_b.TaskID,
        AssignedAt=now,
    )
    print("TaskAssignments:", ta1, ta2, ta3, ta4)

    sup1 = SupportRequests.create(
        RobotID=robot_1.RobotID,
        IssueDetails="Support for robot 1",
        TimeReported=now,
    )
    sup2 = SupportRequests.create(
        RobotID=robot_3.RobotID,
        IssueDetails="Support for robot 3",
        TimeReported=now,
    )
    print("SupportRequests:", sup1, sup2)

    # ------------------------------------------------------------------
    print("\n" + "=" * 70)
    print("MANY-TO-ONE — sample Relationship loads")
    print("=" * 70)

    print("robot_1.deployment_location (expect deployment 1):", robot_1.deployment_location)
    print("robot_3.deployment_location (expect deployment 2):", robot_3.deployment_location)
    print("robot_1.owner_user (expect John):", robot_1.owner_user)
    print("robot_4.owner_user (expect Michael):", robot_4.owner_user)
    print("robot_1.robot_model / software_version:", robot_1.robot_model, robot_1.software_version)

    print("ar1.ai_model / ar1.robot:", ar1.ai_model, ar1.robot)
    print("resp1.ai_request / resp1.task:", resp1.ai_request, resp1.task)
    print("Chain resp1 → request → robot:", resp1.ai_request.robot)

    print("ta1.robot / ta1.user / ta1.task:", ta1.robot, ta1.user, ta1.task)
    print("sup1.robot:", sup1.robot)

    # ------------------------------------------------------------------
    print("\n" + "=" * 70)
    print("BACKREFERENCE — lists on parent (same Users row via identity map)")
    print("=" * 70)

    # Touch owner on each of John’s robots so Users.robots collects both (same john instance as create()).

    print("robot_1.user is john?", robot_1.user is john)
    print("john.robots (expect John’s two robots):", john.robots)

   
    print("michael.robots (expect Michael’s two robots):", michael.robots)

    # Same pattern as john.robots: each robot’s deployment_location Relationship appends
    # this robot to DeploymentLocations.robots (identity map → same object as deployment_1 / deployment_2).

    print("deployment_1 is robot_1.deployment_location?", deployment_1 is robot_1.deployment_location)
    print("deployment_1.robots (expect both robots on site 1):", deployment_1.robots)


    print("deployment_2 is robot_3.deployment_location?", deployment_2 is robot_3.deployment_location)
    print("deployment_2.robots (expect both robots on site 2):", deployment_2.robots)

    m1_via_ar1 = ar1.ai_model
    print("AI Model 1 → .ai_requests:", getattr(m1_via_ar1, "ai_requests", "<none>"))

    req_via_resp = resp2.ai_request
    print("AIRequest (via resp2) → .ai_responses:", getattr(req_via_resp, "ai_responses", "<none>"))

    # ------------------------------------------------------------------
    print("\n" + "=" * 70)
    print("FILTER — two robots per deployment (ORM query)")
    print("=" * 70)

    d1_id = deployment_1.DeploymentLocationID
    d2_id = deployment_2.DeploymentLocationID
    on_dep1 = Robots.filter(DeploymentLocationID=d1_id)
    on_dep2 = Robots.filter(DeploymentLocationID=d2_id)
    print("Robots on deployment 1 (expect 2):", len(on_dep1), on_dep1)
    print("Robots on deployment 2 (expect 2):", len(on_dep2), on_dep2)

    # ------------------------------------------------------------------
    print("\n" + "=" * 70)
    print("LAZY CACHE — robot_2.user read twice (delegates to owner_user)")
    print("=" * 70)

    _ = robot_2.user
    assert hasattr(robot_2, "_owner_user_cache")
    print("second read:", robot_2.user)

    print("\n" + "=" * 70)
    print("DONE")
    print("=" * 70)
    return 0


    print("robot.where(RobotID=1):", Robots.where(RobotID=1))
   

if __name__ == "__main__":
    raise SystemExit(main())
