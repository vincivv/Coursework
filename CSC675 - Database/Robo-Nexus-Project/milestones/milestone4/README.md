# Milestone 4 — Data Access and Service API Implementation  

### 15 Points


![Phase](https://img.shields.io/badge/Phase-SQL%20Encapsulation-blue)
![Tool](https://img.shields.io/badge/Tool-ORM%20and%20Service%20Layer-brown)
![Language](https://img.shields.io/badge/Language-Python-green)
![Database](https://img.shields.io/badge/Database-MySQL-blue)
![Work](https://img.shields.io/badge/Work-Team--Based-red)

---

## Purpose

The goal of this milestone is to design and implement a domain-specific data access and service API for your project.

You are not building a general-purpose ORM such as SQLAlchemy. Instead, you will implement a narrow, domain-specific 
interface composed of three layers; an ORM layer, domain models, and services. Together, these layers encapsulate all SQL 

access and expose a clean API that other developers or applications can use without writing raw SQL.

This milestone brings together schema design, normalization, approved business rules, and application-level 
abstractions into a cohesive interface similar to what real software teams build when multiple applications need to 
interact with the same database through stable, well-defined APIs.

---

## Usage

In real software companies, raw SQL code and database behavior are encapsulated across three main layers; ORM, models, 
and services. This separation allows teams to scale systems, expose data safely through APIs, and evolve database 
schemas without breaking applications.

### Object Relational Mapping Tools (ORM)

The ORM layer translates database tables and relationships into objects the application can work with. It generates SQL 

behind the scenes, enforces constraints defined in the schema, and isolates the application from database-specific 
syntax and implementation details.

### Models

Models represent the core domain concepts of the system, such as users, orders, or payments. They 
define structure, relationships, and validation rules, and serve as the shared language between the database and the 
rest of the application.

### Services

Services contain business logic and orchestrate interactions between models. They are the layer typically 
exposed through APIs, ensuring that multiple applications can interact with the database consistently without 
direct access to SQL or schema details.

---

## Ownership and Evaluation

This is a team-based milestone.

All code submitted must be fully understood and owned by the team members who contributed to it. During in-class 
team meetings, you will be asked to explain:

- how models map to tables,
- how relationships and backreferences are implemented,
- how tables are created dynamically,
- and how service methods interact with the database safely.

Even perfectly working code will not receive credit if you cannot explain how it works and why it was 
designed that way. Understanding and ownership matter more than output.

---

## Language Requirement

This milestone must be implemented in Python.

Python is used because it allows clear discussion of ORM concepts, is widely used in industry, and matches the examples 
discussed in class. You are not expected to be a Python expert, but you are expected to learn what you need and apply 
it thoughtfully.

---

## No Template Code Policy

No starter or template code is provided for this milestone.

This is intentional. You will see ORM concepts discussed and demonstrated in class, but you are expected to design 
and implement your own solution. This mirrors how engineers build internal tooling in real teams.

---

## Expected Project Structure

Your implementation must include a clear separation between:
- internal ORM components,
- domain models,
- and the services layer.

```text
/orm
├── base.py
├── domains.py
├── datatypes.py
├── relationships.py
├── migrations.py
/models
├── models.py
/services
├── services.py
tests.py
```

You may add additional files if they help clarify your design, but these files are required.

---

## File Responsibilities

**base.py**  
Contains the core ORM logic. Responsible for model registration, metadata handling, CRUD operations, and database interaction.

**domains.py**  
Defines domain-level constraints and reusable logic that reflect the semantics of your schema and approved business rules.

**datatypes.py**  
Defines how Python types map to SQL types, including validation, serialization, and constraint enforcement.

**relationships.py**  
Implements relationships and backreferences in a way similar to SQLAlchemy, but tailored to your schema and design decisions.

**migrations.py**  
Implements basic migration support. At a minimum, your ORM must be able to detect new models and create corresponding 
tables automatically. This does not need to be a full migration framework.

**models.py**  
Defines the domain models that map directly to database tables. Each model specifies fields, keys, and relationships 
using your ORM abstractions. Models must not contain raw SQL and must rely entirely on the ORM layer for persistence and querying.

**services.py**  
Defines the public API of your database. Contains business logic and service methods that orchestrate interactions 
between models. All database access must occur through this layer; application code should never interact directly with models or SQL.

**tests.py**  
Contains test scripts used to validate the ORM, models, and services. Tests should demonstrate table creation, 
data insertion and updates, querying through ORM methods, relationship handling, and correct behavior of service-layer operations.


---

## Models Layer

You must provide a `models.py` file defining classes that represent your database tables.

Requirements:

- Each model maps to one table.
- Models define fields, keys, and relationships.
- Models inherit from your ORM base class.
- Models must not contain raw SQL.

Your ORM must support dynamic table creation based on model definitions.

#### Illustrative example of different models:

```python
class User(Model):
    __tablename__ = "users"

    id = Integer(primary_key=True)
    first_name = String(nullable=False)
    last_name = String(nullable=False)
    email = String(unique=True, nullable=False)

    # One-to-many relationship
    orders = Relationship(
        target="Order",
        backref="user",
        lazy=True
    )


class Order(Model):
    __tablename__ = "orders"

    id = Integer(primary_key=True)
    user_id = Integer(foreign_key="users.id", nullable=False)
    status = String(nullable=False)
    created_at = DateTime(nullable=False)

    # One-to-many relationship
    items = Relationship(
        target="OrderItem",
        backref="order",
        lazy=True
    )


class OrderItem(Model):
    __tablename__ = "order_items"

    id = Integer(primary_key=True)
    order_id = Integer(foreign_key="orders.id", nullable=False)
    product_name = String(nullable=False)
    quantity = Integer(nullable=False)
    price = Decimal(nullable=False)

    

```
---

## Services Layer (Public API)

You must implement a `services.py` file that represents the **public interface** of your database.

This is the layer intended for application developers. All database interaction must go through service methods. No raw 
SQL should appear in your service methods.

#### Illustrative Service Example (Note, clean API, no SQL)

```python
def create_user(first_name, last_name, email):
    """
    Creates a new user and persists it to the database.
    """
    user = User(
        first_name=first_name,
        last_name=last_name,
        email=email
    )
    user.save()
    return user


def create_order_for_user(user_id, items):
    """
    Creates an order for an existing user.

    items is expected to be a list of dictionaries:
    [
        {"product_name": "Keyboard", "quantity": 1, "price": 99.99},
        {"product_name": "Mouse", "quantity": 2, "price": 29.99}
    ]
    """
    order = Order(
        user_id=user_id,
        status="CREATED",
        created_at=now()
    )
    order.save()

    for item in items:
        OrderItem(
            order_id=order.id,
            product_name=item["product_name"],
            quantity=item["quantity"],
            price=item["price"]
        ).save()

    return order


def get_user_orders(user_id):
    """
    Retrieves all orders for a given user using ORM relationships.
    """
    user = User.get(id=user_id)
    return user.orders


def get_order_details(order_id):
    """
    Retrieves an order and all its items using backreferences.
    """
    order = Order.get(id=order_id)
    return {
        "order_id": order.id,
        "status": order.status,
        "items": order.items
    }

```

The goal is that a user of your database does not need to understand SQL to interact with it.

---

## Testing Expectations

You must test your models and services using your own test scripts based on your ORM implementations

Your tests should demonstrate:

- table creation from models,
- inserting and updating records,
- querying data via ORM methods,
- relationship loading and backreferences,
- and service-layer usage.

Tests do not need to be formal unit tests, but they must be clear, meaningful, and free of errors.

---

## Grading Overview (15 Points)

This milestone is graded holistically.

Evaluation focuses on:

- quality and coherence of the ORM design,
- alignment between schema, ORM, and services,
- correctness of dynamic table creation,
- quality of abstractions and separation of concerns,
- good support on model creation based on your ORM design
- good and appropriate service methods for the final user of your API based on your application's requirements
- and demonstrated ownership during in-class discussions.

Feature count is less important than system design and understanding.

---

## Submission Guidelines

This milestone follows the same **two-version submission model** used in previous milestones.

All work must be committed to your team repository under `milestone4/` directory

---

### Version 1 — Internal ORM Design Review

Version 1 is a **design and implementation checkpoint**.

By the Version 1 deadline, your repository must include:

- All internal ORM files (`base.py`, `domains.py`, `datatypes.py`, `relationships.py`, `migrations.py`, and any supporting files).
- Code that demonstrates how models *will* be supported, even if `models.py` and `services.py` are not yet complete.

During in-class team meetings, we will review:
- your internal ORM structure,
- how models are registered and mapped,
- how relationships and backreferences are handled,
- how dynamic table creation and migrations are approached.

This version is not graded, but feedback given here is expected to be incorporated into Version 2.

---

### Version 2 — Final ORM, Models, and Services

Version 2 is the **final graded submission**.

By the Version 2 deadline, your repository must include:

- All internal ORM implementation files.
- A complete `models.py` defining your database models.
- A complete `services.py` exposing a clean, SQL-free API for users of your database.
- Any test scripts used to validate functionality.
- A `contributions.md` file describing individual contributions.

At this stage, your ORM must be fully functional end-to-end.

For grading purposes, testing and evaluation will focus primarily on:
- `models.py` and `services.py`,
- but these files must work correctly **using your internal ORM implementation**.

If supporting files are missing or incomplete, `models.py` and `services.py` will not be considered functional, and the 
milestone will be considered incomplete.

---

### General Rules

- All required files must be present in the repository at the time of grading.
- You may organize files into folders as needed, as long as the structure is clear and imports resolve correctly.
- You may author documentation or notes in any format, but code must run as committed.
- Only work merged into the `master` branch through a pull request approved by the team lead will be considered.

Incomplete repositories, missing internal ORM files, or non-functional imports will result in a non-passing evaluation 
for this milestone.

---

## Commit and Review Policy

For grading purposes, only work merged into the `master` branch through a pull request approved by the team lead will be considered.

Commits pushed directly to `master`, or merged without approval, will not be counted.

Approval confirms review, not correctness. Ownership and understanding are evaluated by the instructor during discussions.

---

## Final Notes

This milestone is intentionally very challenging.

If done well, you will understand not just how ORMs are used, but how they work internally and where their 
limitations are. Approach this milestone the way engineers approach internal tooling: design carefully, implement 
deliberately, and be ready to defend every decision.

---

