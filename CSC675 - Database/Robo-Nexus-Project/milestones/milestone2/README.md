# Milestone 2 — Database Normalization and Schema  

### 15 Points


![Phase](https://img.shields.io/badge/Phase-Logical%20Design-blue)
![Database](https://img.shields.io/badge/Database-MySQL-green)
![Process](https://img.shields.io/badge/Process-Normalization-important)
![Modeling](https://img.shields.io/badge/Modeling-DrawIO-lightgrey)
![Work](https://img.shields.io/badge/Work-Team--Based-important)

---

## Purpose

The goal of this milestone is to turn the **conceptual design** from Milestone 1 into a **clean, well-structured relational schema**.

In this milestone, you will:
- normalize your design to reduce redundancy and improve integrity,
- make deliberate design tradeoffs where appropriate,
- and translate your design into a concrete database schema.

This is where many real-world database designs succeed or fail. Normalization is not a mechanical checklist; it is a design process that requires judgment.

---

## Ownership and Evaluation

This is a **team-based milestone**.

All work submitted must be **fully understood and owned** by the team members who contributed to it. During in-class team meetings, you will be asked to explain:

- why certain tables were normalized or left denormalized,
- how functional dependencies were identified,
- and how your schema reflects the requirements from Milestone 1.

If a team member cannot explain or defend part of the work they are associated with, that work will not be considered theirs for grading purposes.

Submitting SQL files is not enough. **You are expected to understand your schema and the decisions behind it.**

---

## Two-Version Submission Model

Milestone 2 is submitted in **two versions**:

- **Version 1 (for review)**  
  Submitted by the Milestone 2 deadline.  
  Used for in-class review and discussion.  
  This version is not final.

- **Version 2 (for grading)**  
  Submitted on the same day you submit **Version 1 of Milestone 3**.  
  This version must reflect feedback discussed during team meetings.  
  Only Version 2 is used for final grading.

Both versions must be committed to this repository following the submission and commit policies below.

---

## What to Include

Your Milestone 2 submission must include the following components.

---

### 1. Database Normalization

Using your EER and entity descriptions from Milestone 1, normalize your design into a set of well-structured relational tables.

You are expected to apply normalization **systematically and deliberately**, not mechanically. At a minimum, your 
schema must be normalized through **1NF, 2NF, and 3NF**, and where applicable, you must also apply **BCNF and 4NF**.

This means you are responsible for:

- Identifying functional dependencies and multivalued dependencies.
- Explaining why certain decompositions were necessary.
- Justifying cases where a table does *not* require further normalization.

Normalization is about **reducing redundancy and improving integrity**, but it is not free. In some cases, 
further normalization may introduce performance or complexity tradeoffs. When that happens, you must be 
able to clearly explain why your final design makes sense for your system.

Your submission should make it clear:

- how entities from the conceptual model were transformed into relations,
- how each normal form was applied (or why it was not needed),
- and how the final schema preserves the requirements defined in Milestone 1.

During in-class team meetings, you will be asked to walk through your normalization decisions and defend 
them. If you cannot explain why a table is in a given normal form, that table will be treated as incorrectly 
normalized for grading purposes.


---

### 2. Normalized EERs Diagram

Provide an EER diagram that represents your normalized conceptual design. This is the final EER that will be used in
technical discussions regarding the final architecture and design of your database.

Requirements:
- Use Crow’s Foot notation.
- Ensure the diagram matches your actual schema.
- The diagram must be readable and complete.

You must include:
- The source file (e.g., `eer_normalized.drawio`),
- An exported image (PNG or PDF) embedded in your `m2.pdf`. 

---

### 3. Database Schema Design

Based on your normalized design, create a relational database schema that accurately reflects your decisions.

You are expected to approach this step using **forward engineering** whenever possible. This means starting from 
your normalized tables and deliberately translating them into SQL, paying close attention to table creation order, 
primary and foreign keys, and constraints.

**Reverse engineering is allowed**, but it is not the default expectation. Teams that choose to reverse engineer 
must be able to clearly explain:

- how the schema was created,
- why tables and constraints appear in the order they do,
- and how the resulting schema matches the normalized design from Section 1.

During in-class team meetings, you may be asked to justify why reverse engineering was appropriate for your workflow.

Regardless of the approach used, the final schema must:

- be consistent with your normalized design,
- include correct primary keys, foreign keys, and constraints,
- and execute without errors.

The choice of approach does not reduce expectations. Ownership and understanding of the schema are required in all cases.


---

### 4. SQL Schema and Sample Data

You must provide:

- `schema.sql`  
  Contains all `DROP`, `CREATE DATABASE`, `CREATE TABLE`, and constraint definitions.

- `inserts.sql`  
  Contains sample data used to validate that the schema works correctly.

You are allowed to use AI tools to help generate **sample data** for `inserts.sql`. However, you are still 
responsible for understanding the structure of the data and ensuring it matches your schema.

Your database must run cleanly when executing `schema.sql` followed by `inserts.sql`.

---

## Contributions

Each team must include a `contributions.md` file describing **individual contributions** for this milestone.

Guidelines:
- Be specific about what you worked on.
- Avoid vague statements such as “worked on normalization.”
- Contributions will be verified against commit history and in-class discussions.

Unclear or inaccurate contribution statements may result in a non-satisfactory evaluation.

---

## Grading Overview (15 Points)

Milestone 2 is graded **holistically**, not as a checklist.

The evaluation focuses on:

- Correct and thoughtful application of normalization
- Alignment between requirements, normalized design, and schema
- Quality and clarity of schema design
- Correctness of SQL files
- Professionalism of documentation
- Demonstrated ownership during team meetings

Points are awarded based on the overall quality and coherence of the work, not simply for the submission of your work.

---

## Submission Guidelines

All submissions must be made **through this repository**.

Required files:

- `m2.pdf` — Technical document describing normalization decisions, Final exported normalized EER diagram image embedded,
   and database schema diagram
- `schema.sql` — SQL schema definition
- `inserts.sql` — Sample data
- `eer_normalized.drawio` — Source diagram
- `contributions.md` — Individual contributions

Document format:
- You may author your documentation in any format you prefer.
- **Only the PDF version (`m2.pdf`) and (`sql`) files will be used for grading.**

All files must be committed to the `master` branch.

---

## Commit and Review Policy

For grading purposes, only work merged into the `master` branch through a pull request **approved by the team lead** will be considered.

Commits pushed directly to `master`, or changes merged without team lead approval, will not be counted.

The team lead is responsible for reviewing work for coherence and completeness, not for grading. Technical 
understanding and ownership are evaluated by the instructor during in-class discussions.

---

## Final Notes

Normalization and schema design are where database projects often become either fragile or robust. In most cases, 
normalization reduces data redundancy and improves data integrity, but it can also introduce 
performance tradeoffs. 

Take the time to reason through your decisions, discuss tradeoffs with your team, and be prepared to explain why 
your design makes sense. Strong work here will make the remaining milestones far more manageable.

Approach this milestone the way professional engineers approach schema design: carefully, deliberately, and 
with full ownership of your decisions.
