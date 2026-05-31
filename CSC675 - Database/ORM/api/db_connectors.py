import mysql.connector
from dotenv import load_dotenv
import os

# Load environment variables from .env file
load_dotenv()

class MySQL:
    @staticmethod
    def instance():
        """
        Establishes and returns a connection and cursor to a MySQL database.

        Returns:
            tuple: (connection, cursor) where:
                - connection is mysql.connector connection object
                - cursor is a cursor object to execute SQL queries

        Raises:
            mysql.connector.Error: If there is an error during connection
        """
        connection = mysql.connector.connect(
            host=os.getenv("DB_HOST"),
            user=os.getenv("DB_USER"),
            password=os.getenv("DB_PASSWORD"),
            database=os.getenv("DB_NAME")
        )

        # Return rows as dictionaries (not tuples), which allows key-based access to column values
        cursor = connection.cursor(dictionary=True)
        return connection, cursor













