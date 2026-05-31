# models.py
#
# This file is where you define your ORM models. Models represent tables in the database,
# and each instance of a model corresponds to a row in that table.
#
# Models should inherit from the `Base` class, which provides methods for interacting with the database,
# such as saving records, querying, and deleting.
#
# In this file, you will:
#   - Define your own models, from your database schema in this project, by subclassing the `Base` class.
#   - Use `Column` objects to define columns and their types (e.g., `Integer`, `String`).
#   - Add attributes to each model class to represent columns in the corresponding database table.
#   - Define additional methods in the models as necessary for specific functionality (e.g., custom queries,
#     business logic, etc.).
#
#
# Students should implement their own models, specifying the columns using `Column` and selecting the appropriate
# `types` for each column, such as `Integer`, `String`, `Boolean`, etc.
#

from api.columns import Column
from api.datatypes import Integer, String, Boolean
from api.base import Base
from datetime import date

from api.relations import Relation


class Artist(Base):
    # match exactly the name of your table (case-sensitive)
    __name__ = "Artist"

    # properties they need to match exactly the names of your
    # attributes in this table

    artist_id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    publisher = "Jose"

    def __repr__(self):
        return f"Artist(artist_id: {self.artist_id}, name: {self.name})"

class Album(Base):
    __name__ = 'Album'
    album_id = Column(Integer, primary_key=True)
    title = Column(String, nullable=False)
    year_released = Column(String, nullable=False)

    def __repr__(self):
        return f"Album(album_id: {self.album_id}, title: {self.title})"



class Track(Base):
    __name__ = "Track"

    track_id = Column(Integer, primary_key=True)
    title = Column(String, nullable=False)
    album_id = Column(Integer, nullable=False, foreign_key=True)
    artist_id = Column(Integer, nullable=False, foreign_key=True)
    genre_id = Column(Integer, nullable=False, foreign_key=True)

    album_object = Relation(model_name='Album',
                            foreign_key='album_id',
                            backreference='track',
                            lazy=False)
    artist_object = Relation(model_name='Artist',
                             foreign_key='artist_id',
                             backreference='artist',
                             lazy=True)










