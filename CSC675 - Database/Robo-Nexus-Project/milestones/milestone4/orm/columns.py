# columns.py
#
# This file defines the `Column` class used in the ORM framework. Each instance of
# the `Column` class represents a single column in a database table. The `Column` class
# allows us to define the column's data type, constraints (e.g., primary key, nullable, etc.),
# and foreign key relationships.
#
# The `Column` class is used by the ORM framework to map the attributes of Python classes
# (like `User`, `Post`, etc.) to actual database tables. The ORM will automatically generate
# the necessary SQL queries based on these column definitions.
#
# The `Column` class handles attributes such as:
#   - `column_type`: The type of the column (e.g., INTEGER, TEXT).
#   - `primary_key`: Whether the column is part of the primary key.
#   - `nullable`: Whether the column can be null.
#   - `unique`: Whether the column values must be unique.
#   - `foreign_key`: The foreign key constraint that relates to another table.
#
# Students need to implement the following methods to complete the functionality of this class:
#   - `get_sql()`: Generates the SQL representation of the column.
#   - `validate_type()`: Validates that the column's type is supported.
#   - `is_primary_key()`: Checks if the column is part of the primary key.
#   - `to_dict()`: Returns the column's configuration as a dictionary.
#   - `get_constraints()`: Generates a string of the column's constraints (e.g., "NOT NULL").
#   - `is_foreign_key()`: Checks if the column is a foreign key.
#
# Example usage in the ORM base class:
#
#   class User:
#       id = Column(Integer, primary_key=True)  # Primary key column of type Integer
#       name = Column(String(255), nullable=False)  # String column, not nullable
#       email = Column(String(100), unique=True)  # String column, with a unique constraint
#       created_at = Column(Date)  # Date column
#       profile_id = Column(Integer, foreign_key='Profile(id)')  # or foreign_key=True to mark only
#
#   The ORM will use these `Column` instances to define the table schema and generate the
#   corresponding SQL for table creation, validation, and foreign key enforcement.


class Column:
    def __init__(self, column_type, primary_key=False, nullable=True, unique=False, foreign_key=None, default=True, on_delete=None, on_update=None, auto_increment=False):
        self.type = column_type
        self.primary_key = primary_key
        self.nullable = nullable
        self.unique = unique
        self.foreign_key = foreign_key
        self.on_update = on_update
        self.on_delete = on_delete
        self.default = default
        self.auto_increment = auto_increment

    def to_sql(self):
        column_type = self.type() if isinstance(self.type, type) else self.type
        type_sql = column_type.to_sql() if hasattr(column_type, "to_sql") else str(column_type)
        constraints = self.get_constraints()
        return f"{type_sql} {constraints}".strip()

    def validate_type(self):
        return self.type is not None


    def is_primary_key(self):
        if self.primary_key:
            return True
        return False

    def to_dict(self):
        return {
            "type": self.type,
            "primary_key": self.primary_key,
            "nullable": self.nullable,
            "unique": self.unique,
            "foreign_key": self.foreign_key,
            "default": self.default,
            "on_delete": self.on_delete,
            "on_update": self.on_update,
            "auto_increment": self.auto_increment,
        }

    def get_constraints(self):
        constraints = []
        if self.primary_key:
            constraints.append("PRIMARY KEY")
        if not self.nullable:
            constraints.append("NOT NULL")
        if self.unique:
            constraints.append("UNIQUE")
        if self.auto_increment:
            constraints.append("AUTO_INCREMENT")
        if isinstance(self.foreign_key, str) and self.foreign_key:
            constraints.append(f"REFERENCES {self.foreign_key}")
            if self.on_delete:
                constraints.append(f"ON DELETE {self.on_delete}")
            if self.on_update:
                constraints.append(f"ON UPDATE {self.on_update}")
        return " ".join(constraints)

    def is_foreign_key(self):
        if self.foreign_key is True:
            return True
        if isinstance(self.foreign_key, str) and self.foreign_key:
            return True
        return False