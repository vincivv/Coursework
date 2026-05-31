# Milestone 3 — Business Rules and Database Logic  

### 20 Points

![Phase](https://img.shields.io/badge/Phase-Relational-blue)
![Database](https://img.shields.io/badge/Database-MySQL-blue)
![SQL](https://img.shields.io/badge/SQL-Advanced-important)
![Work](https://img.shields.io/badge/Work-Implementation-important)
![Work](https://img.shields.io/badge/Work-Team--Based-important)

---

## Purpose

The goal of this milestone is to **apply your database design to real, meaningful business rules** that reflect the 
unique features of your project.

Up to this point, you have focused on *structure*: entities, relationships, normalization, and schema design. 

In this milestone, you focus on *behavior*: what your database 
actually does to enforce rules, automate logic, and support real use cases.

This is a major checkpoint in the course. From this point on, your database is no longer passive storage; it becomes an 
active system that enforces business logic.

---

## Terminology Note

In industry, what we call *business rules* here are often referred to as:

- business rules,
- database rules,
- data integrity rules,
- or database logic.

All of these refer to **rules that the database itself is responsible for enforcing**, not application code.

---

## Ownership and Evaluation

This milestone is **individual work**, even though it builds on the database designed in earlier team milestones.

Every rule you define and implement must be **fully understood and owned by you**. During in-class meetings, you will be asked to explain:

- why a rule exists,
- what problem it solves,
- why it belongs in the database,
- and why you implemented it the way you did.

Even perfectly working SQL will not receive credit if you cannot explain and defend it.  

**Ownership matters more than correctness alone.**

---

## Two-Phase Structure

Milestone 3 has **two distinct phases**.

### Phase 1 — Business Rules Definition (Design Checkpoint)

In this phase, you define the business rules your database must enforce.

You must create a file named: `business_rules.md`


This file must contain **at least 15 business rules** that:

- are directly tied to the unique features of your project,
- are non-trivial and meaningful,
- and require database-level logic to enforce.

Each business rule must include:

- **Purpose**: What problem does this rule solve?
- **Description**: What must the database enforce?
- **Challenges**: Why is this rule non-trivial?
- **Assumptions**: Any constraints or assumptions being made.
- **Planned Approach**: Which database mechanisms are appropriate (triggers, functions, procedures, events, etc.). No 
  SQL code is needed at this point since this document will be accessed by non-tech oriented readers as well.

This file will be reviewed and discussed with the instructor during a team meeting.  
Only **approved business rules** may be implemented in Phase 2.

This phase exists to prevent wasted work and to ensure your rules make sense *before* implementation.

---

### Phase 2 — Implementation and Justification

Once your business rules are approved, you will implement them.

You must submit:

- `m3.pdf`  
  A document containing:
  - the **final, approved list of business rules**,
  - a justification for each rule,
  - and an explanation of how it relates to your database design and earlier milestones.

- `business_rules.sql`  
  A SQL file implementing all approved business rules. Implementations of non-approved or unreviewed rules will not be 
  considered for credit.

Your implementations may use any combination of:

- triggers,
- stored procedures,
- functions,
- views,
- scheduled events,
- and constraints.

You are expected to use **advanced SQL features** learned in class appropriately and consistently.

---

## Implementation Expectations

Your business rules should demonstrate:

- complex logic and calculations,
- enforcement of real-world constraints,
- dynamic data behavior,
- security and integrity concerns,
- and alignment with your non-functional requirements.

You are encouraged to use multiple SQL mechanisms when appropriate.  
There is no single “correct” implementation, but there must be a **clear rationale** behind your choices.

> **Important:**  
> Examples discussed in class are provided only to illustrate structure and documentation style.  


---

## Files to Submit

This milestone requires **four files**:

- `business_rules.md` — Phase 1 design and approval
- `m3.pdf` — Final approved rules and justifications
- `business_rules.sql` — SQL implementation of all rules
- `contributions.md`  — Detailed list of contributions from all team members. 

All files must be committed to the repository following the policies below.

---

## Grading Overview (20 Points)

Milestone 3 is graded **holistically**.

The evaluation focuses on:

- quality and relevance of business rules,
- alignment with the project’s unique features,
- correctness and robustness of SQL implementations. All the implementations provided must be fully optimized. 
- appropriate use of advanced SQL concepts,
- clarity of explanations and justifications,
- and demonstrated ownership during in-class discussions.

A working implementation alone is not sufficient.  
Understanding and reasoning are required to pass this milestone.

---

## Submission Guidelines

All submissions must be made **through this repository**.

Rules:

- All required files must be present in the repository.
- Incomplete or missing files will result in a non-passing evaluation.

---

## Commit and Review Policy

For grading purposes, only work merged into the `master` branch through a pull request **approved by the team lead** will be considered.

Commits pushed directly to `master`, or merged without approval, will not be counted.

Approval confirms that the work is visible and reviewed, not that it is correct.  
Technical understanding and ownership are evaluated by the instructor during discussions.

---

## Final Notes

This milestone represents a shift from design to implementation.

Strong work here shows that you can reason about real systems, not just schemas. Weak or superficial rules will make 
later milestones significantly harder.

Approach this milestone the way engineers approach production systems:  
define rules carefully, implement them deliberately, and be ready to defend every decision.


