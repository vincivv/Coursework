# datatypes.py
#
# This file defines the data types used in the ORM framework. Each class represents
# a specific column type that can be used in the database schema. These types
# are then used by the ORM's base class to define columns in database tables.
#
# The `Column` class in the ORM will reference these data types to configure the
# column's behavior, such as validating values, determining the SQL representation,
# and enforcing constraints.
#
# For example,
#   - The `Integer` class represents an INTEGER column type in SQL.
#   - The `String` class represents a TEXT column type with an optional length constraint.
#   - The `Boolean` class represents a BOOLEAN column type.
#
# Students should implement the missing methods for each type (e.g., `get_sql`, `validate`)
# to ensure proper integration with the ORM and the generation of valid SQL queries.
#
# Example usage in the ORM base class:
#
#   class User:
#       id = Column(Integer, primary_key=True)   # An INTEGER primary key column
#       name = Column(String(255), nullable=False)  # A TEXT column with a length of 255, not nullable
#       is_active = Column(Boolean, default=True)  # A BOOLEAN column with a default value of True
#       created_at = Column(Date)  # A DATE column
#
#   This would represent a "User" table with columns: "id", "name", "is_active", and "created_at".
#   The ORM will use the data types (Integer, String, Boolean, Date) to validate values and generate
#   SQL queries when interacting with the database.

class Integer:
    def __init__(self, type="INT", length=None):
        self.type = type
        self.length = length

    def validate(self):
        return True

    def to_sql(self):
        return self.type


class String:
    def __init__(self, type="VARCHAR", length=255):
        self.type = type
        self.length = length

    def validate_length(self):
        return self.length is None or (isinstance(self.length, int) and self.length > 0)

    def to_sql(self):
        if self.length:
            return f"{self.type}({self.length})"
        return self.type

class Float:
    def __init__(self, type="FLOAT"):
        self.type = type

    def validate(self):
        return True

    def to_sql(self):
        return self.type

class Boolean:
    def __init__(self):
        self.type = "BOOLEAN"

    def validate(self):
        return True

    def to_sql(self):
        return self.type

class Date:
    def __init__(self, type="DATE"):
        self.type = type

    def validate(self):
        return True

    def to_sql(self):
        return self.type


class DateTime:
    def __init__(self, type="DATETIME"):
        self.type = type

    def validate(self):
        return True

    def to_sql(self):
        return self.type

class Blob:
    def __init__(self):
        self.type = "BLOB"

    def validate(self):
        return True

    def to_sql(self):
        return self.type