# Business rules

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

## Rule 3: AI Response recommending valid role to robot

**Purpose:** Prevent invalid role.

**Description:** AI should recommend the valid roles to the robot based on energy status and hardware specs.

**Challenges:** Needs to join on multiple tables like Robot, RobotRole, Task, AI requests, energy status, hardware.

**Assumptions:** AI can recommend robot roles based on the data available from required tables.

**Planned Approach:** Trigger on AI response for role.

---

## Rule 4: One active task per Robot

**Purpose:** Prevents overloading robots.

**Description:** A robot can only have one active task at a time and must finish the task before getting another active task.

**Challenges:** Can cause race conditions with other robots trying to take roles that are dependent on other roles.

**Assumptions:** A robot can not be multitasking.

**Planned Approach:** Triggers on Task Execution.

---

## Rule 5: Set Robot Active Time

**Purpose:** Prevent interference with users and other robots.

**Description:** When assigned a task a robot shall also need a time to begin executing the task, making sure the robot doesn’t crash into another one or interfere with other activities depending on the deployment location.

**Challenges:** Can cause delays in execution of all tasks across robot fleet.

**Assumptions:** Robots can move, and will not always be monitored by a user.

**Planned Approach:** Triggers on task assignment (A robot cannot execute a task that overlaps with another robot or user path preventing assignment altogether).

---

## Rule 6: Robot Cannot Execute Task without Assignment by user or AI

**Purpose:** Prevent Robot from unauthorized execution.

**Description:** A robot shall not execute a task by itself unless the robot has a senior role or assigned by a user or by AI to make sure the robot gets tasks as per their specs and status.

**Challenges:** Checking multiple tables before assigning.

**Assumptions:** TaskAssignment table must exist and contains the tasks needed to be completed.

**Planned Approach:** Trigger on TaskExecutions.

---

## Rule 7: Task Timeout

**Purpose:** Assign a new robot to the task if unseen situations occur.

**Description:** If the robot does not complete the task under specified time, reassign the robot from the task and give it to a new robot.

**Challenges:** Tracking the start time and expected time, and figuring the optimal expected time.

**Assumptions:** Task table must exist and contain the expected time.

**Planned Approach:**

---

## Rule 8: Robot can not charge and execute a task at the same time

**Purpose:** Prevents robots from having multiple states at once.

**Description:** A robot can not be doing a task while charging at the same time.

**Challenges:** A robot might need to start charging while still working on a task.

**Assumptions:** A robot must stop a task execution before starting to charge.

**Planned Approach:** Check for current status before executing a task/charging session.

---

## Rule 9: Robots cannot have multiple active roles

**Purpose:** Enforce one role at a time.

**Description:** A robot shall have only one role during the active sessions assigned by AI or user.

**Challenges:** Must have the details of the robots and also its existing role and also look up multiple tables.

**Assumptions:** Role is stored in RobotRole table and description of robot in robot table.

**Planned Approach:** Trigger on RobotRole.

---

## Rule 10: Maintenance Tiers for the Robot that restrict activity

**Purpose:** To prevent overuse, breakdown or user injury.

**Description:** Robots shall have different maintenance tiers based on how long they’ve been active, how many times they’ve been charged, how many tasks they’ve executed, and how long it’s been since their last maintenance/repair session.

**Challenges:** Determining restrictions and how many tiers till complete restriction.

**Assumptions:** Robot is dangerous when not maintained.

**Planned Approach:** Triggers on all actions incrementing a maintenance tier value every time.

---

## Rule 11: Emotional Record must belong to existing interactions

**Purpose:** Prevent data from corruption and stale.

**Description:** Emotional record shall not exist without interactionSession and user.

**Challenges:** Keeping track of emotion of the users based on the interactions.

**Assumptions:** Robots shall record user emotions in the Emotional record table based on interaction sessions.

**Planned Approach:** Triggers on corresponding tables.

---

## Rule 12: Automatic Role Reassignment

**Purpose:** Ensure continuous coordination if the robot holding a critical role becomes unavailable.

**Description:** If a robot assigned to a critical role becomes unavailable, the system shall assign that role to a new robot.

**Challenges:** Detecting the unavailability, choosing the proper replacement.

**Assumptions:** Robot status is accurate and updated frequently in the Robot table.

**Planned Approach:** Use a trigger on robot status updates.

---

## Rule 13: Analyzing emotional patterns by AI and sending alerts

**Purpose:** To detect emotionally critical situations and notify operators in real time.

**Description:** If an AI model analyzes emotional patterns and detects a high risk emotional state, the system shall automatically generate an alert for the associated robot.

**Challenges:** Requires tracking of emotion and drawing the emotional pattern and also looking up multiple tables.

**Assumptions:** Emotional state has a threshold value and emotional pattern is generated automatically based on emotional record per session.

**Planned Approach:** Trigger on emotional patterns and AI response.

---

## Rule 14: Robot Hardware Compatibility for Task Assignment

**Purpose:** Prevent robots from being assigned tasks they are physically or technically unable to perform.

**Description:** A robot shall not be assigned or begin execution of a task unless its hardware specifications meet all task requirements.

**Challenges:** This is non-trivial because it requires comparing task requirements against robot capabilities, possibly across multiple tables such as Robot, HardwareSpecs, Task, and TaskRequirements. It may also involve checking several attributes at once, such as arm type, mobility, sensor availability, lifting capacity, or processing capability.

**Assumptions:** Each task has clearly defined hardware requirements, and each robot has up-to-date hardware specification records stored in the database.

**Planned Approach:** Trigger or stored procedure on task assignment and/or task execution that validates the assigned robot against the task’s required hardware profile before allowing the record.

---

## Rule 15: Collision Avoidance Between Robots During Task Execution

**Purpose:** Prevent physical collisions and ensure safe navigation when multiple robots are operating simultaneously.

**Description:** Two or more robots shall not be assigned tasks that result in overlapping paths or conflicting spatial zones at the same time.

**Challenges:** This is non-trivial because it requires analyzing robot movement paths, locations, and timing across multiple active tasks. It may involve checking time windows, coordinates, and predicted paths stored in different tables.

**Assumptions:** Each task includes location data (zones, coordinates, or paths), and robot positions are tracked or estimated in the system.

**Planned Approach:** Trigger or procedure on task assignment that checks for spatial and temporal conflicts with other active tasks before allowing the assignment.
