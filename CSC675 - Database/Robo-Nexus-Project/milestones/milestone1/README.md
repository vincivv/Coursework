# Milestone 1 — Database Requirements & Conceptual Design 

### 10 points

![Phase](https://img.shields.io/badge/Phase-Conceptual%20Design-blue)
![Modeling](https://img.shields.io/badge/Modeling-DrawIO-lightgrey)
![Deliverable](https://img.shields.io/badge/Deliverable-Design%20Document-orange)
![Work](https://img.shields.io/badge/Work-Team--Based-important)

---

## Purpose

The goal of this milestone is to clearly define the **problem, requirements, and conceptual design** of your database system.

At this stage, you are not writing code. You are thinking like engineers who need to understand a problem before 
building a solution. Your task is to translate user needs into well-defined database requirements and a solid conceptual
model that can guide future implementation.

This is how database systems are designed in real software teams, and this milestone is meant to reflect that process.

---

## Ownership and Evaluation

This is a **team-based milestone**.

Any work you submit must be fully understood by the team members who contributed to it. During in-class team meetings, I 
will ask questions about your design decisions, assumptions, relationships, and tradeoffs.

If someone cannot explain or defend a part of the work they are responsible for, that work will not be considered 
theirs for grading purposes.

Submitting files is not enough. **You are expected to understand and own what you submit.**

---

## Two-Version Submission Model

Milestone 1 is submitted in **two versions**.

- **Version 1 (for review)**  
  Submitted by the Milestone 1 deadline.  
  This version is used for in-class discussion and feedback. It is not final.

- **Version 2 (for grading)**  
  Submitted on the same day you submit **Version 1 of Milestone 2**.  
  This version must reflect the feedback discussed during team meetings.  
  Only Version 2 is used for final grading.

Both versions must be committed to this repository following the guidelines below.

---

## What to Include

Your Milestone 1 document must include the following sections.

### 1. Cover Page

Include:

- Project title  
- Team alias  
- Team members’ names and SFSU emails  
- GitHub repository link  
- A version history table  

Example:

| Version | Description |
|--------|-------------|
| v1     | Initial submission for review |
| v2     | Revised submission after feedback |

---

### 2. Table of Contents

Include a table of contents with clickable links and page numbers.

This document will be read by both technical and non-technical people, such as engineers, project managers, 
and technical leads. Clear navigation matters.

---

### 3. Project Description

Provide a high-level description of your database system.

For this section a complete competitive analysis of other similar products in the market is required. Your findings will
be included in your project description.

Address the following:

- What problem does your database system solve?  
- Why is this problem worth solving?  
- Describe a set of unique features that other competitors do not have based on your competitive analysis and explain why 
  your system will be different or better than existing solutions in the same domain.
- Name at least two existing tools or systems that would benefit from your database and explain why.

This section should be readable by someone without a technical background.

---

### 4. Functional Database Requirements

Functional requirements describe **what the database system must do**.

Requirements for this section:

- At least **40 entities**, must include weak, associative-ternary and strong entities
- Each entity must have **at least 3 functional requirements**  
- Requirements must be grouped by the entity performing the action  
- Across all requirements, you must cover:
  - One-to-One relationships  
  - One-to-Many / Many-to-One relationships  
  - Many-to-Many relationships  
  - ISA (specialization and generalization)  
  - Aggregation  
  - Recursive relationships  

Use clear and consistent language (for example, “shall”).

> **Important:**  
> Any examples shown in class or mentioned here are meant only to illustrate concepts. They are intentionally 
  simple and are not templates you can reuse. Your project must define its own structure and relationships.

---

### 5. Non-Functional Database Requirements

Non-functional requirements describe **constraints and quality attributes** of your system.

Provide **at least three requirements** for each of the following categories:

- Performance  
- Security  
- Scalability  
- Reliability and availability  
- Storage  
- Privacy  
- Maintainability and coding standards  

Requirements should make sense for your system and be realistic, not generic.

---

### 6. Conceptual Design — Enhanced Entity-Relationship Diagram (EER)

Create a complete **Enhanced Entity-Relationship Diagram (EER)** that represents the conceptual design of your database.

Requirements:

- Use **Crow’s Foot notation** 
- Create an ERD first, then refine it into an EER. You only need to submit the final EER, but we will use your ERD 
  during class discussions to identify and address early design issues. AI generated EERs are allowed, but you are fully responsible for 
  ensuring it follows exactly the EER requirements and notation covered in class. If you use AI tools at any stage, you must 
  first provide them with the final functional requirements, non-functional requirements, and entity set descriptions 
  from the next section so the generated design reflects the full system context; poorly contextualized AI output will 
  be treated as incomplete or incorrect design work. 
- Include entities, attributes, relationships, and cardinalities  
- Use enhanced features where appropriate (for example, ISA, weak entities, aggregation)  
- Ensure the diagram matches your functional and non-functional requirements  

Tooling rules:

- You must use **draw.io** even if your EERs was AI generated.    
- Hand-drawn diagrams are not allowed  
- Your diagrams must follow the notation and format taught in class. Diagrams that use a different format will not be accepted.

Deliverables:

- Save the source file as `eer.drawio`  
- Export the diagram (PDF, PNG, or JPG) and embed it in your document  
- Screenshots are acceptable only if they are clear and readable  

If the diagram is unreadable, missing, or inconsistent with your requirements, this section will receive no credit.

---

### 7. Entity Set Descriptions

**Policy:** At least 40 entities, as described in section 4.

Describe each entity set shown in your EER.

For each entity:

- State whether it is strong or weak  
- Identify key attributes (primary keys, foreign keys, unique constraints)  
- Define attribute types and domains  
- Use meaningful names, especially for associative entities  

This section should be completed **after** the EER is finalized.

---

## Contributions

Each team must include a `contributions.md` file describing **what each team member contributed** to this milestone.

Guidelines:

- Be specific about what you worked on  
- Vague descriptions such as “helped with design” are not acceptable  
- Contributions will be checked against commit history and in-class discussions  

If contributions are unclear or inaccurate, the milestone may be graded as non-satisfactory.

---

## Grading Overview (10 Points)

Milestone 1 is graded as a whole, not as a checklist.

The evaluation considers:

- How clear and complete your requirements are  
- The quality and consistency of your conceptual design  
- How well the requirements and EER align  
- The overall professionalism of the document  
- Your ability to take ownership, explain and defend your work during team meetings  

---

## Submission Guidelines

All submissions must be made **through this repository**.

Required files:

- `m1.pdf` — the full technical document, You are free to create your documentation in any format you prefer 
  (for example, rendered Markdown). However, only the PDF version of your documentation will be used for grading. 
- `eer.drawio` — the source file for your EER diagram. AI generated content is allowed, but the final EER must be 
   created in draw.io and conform exactly to the EER notation and requirements covered in class.  
- `contributions.md` — individual contribution descriptions  

Rules:

- All files must be committed to the `master` branch under `milestone1/` directory
- Version 1 and Version 2 must be clearly labeled in the document and commit history.   
- Incomplete submissions will not be reviewed for feedback or graded. 

---

## Commit and Review Policy

For grading purposes, only work merged into the `master` branch through a pull request **approved by the team lead** will be considered.

Commits pushed directly to `master`, or changes merged without team lead approval, will not be counted toward grading.

The team lead is responsible for reviewing contributions for coherence and completeness, not for grading. The instructor evaluates technical understanding and ownership during in-class discussions.

---

## Final Notes

This milestone sets the foundation for the entire project.

If you take the time to do this well, later milestones will be much easier. If you rush it, problems 
will carry forward and compound.

Approach this milestone the way professional engineers approach early design work: thoughtfully, 
deliberately, and with full ownership of your decisions.
