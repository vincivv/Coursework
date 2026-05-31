# This file is to perform tests to models such as getting a record, inserting, updating ...
from models.models import Track

# All we implemented in class:


# gets the record with id track_id = 7 from the table Track in the database
track7 = Track.get(id=7)

# gets all the tracks from Track table
tracks = Track.all()

# Insert a new track record in table Track
# Note: for this to work, your pk in the database must be set to AUTO_INCREMENT
new_track = Track.create(title="New track", album_id=5, artist_id=1, genre_id=2, length=300)

# update a track
track7.title = "New Updated Title"
track7.save()

# Get the album to which this track is referencing to without doing album = Album.get(id=5)
album5 = track7.album_object

# Get a back reference to track7
track7_reference = album5.track

# check that identity works. In other words, that two different instances of track are not pointing to the same record in the database.
# once an instance pointing to a record in the database is created, all the instances created with the same purpose should return that instance.

track1 = Track.get(id=7)
track2 = Track.get(id=7)

print(track1 is track2) # true because we implemented in the base case identity so track1 = track2
print(track1.album_object is track2.album_object) # true because both instances are the same instance pointing to the same album
print(track1.album_object.track is track2.album_object.track) # true because both backreferences are pointing to the same track instance

# Example service methods so we do not expose the class to the final user of the application
# In this case we added two layers of encapsulation
#    1. We encapsulated SQL code
#    2. We encapsulated class models exposure.
def get_track(track_id):
    return Track.get(id=track_id)

def get_album_by_track(track_id):
    track = get_track(track_id)
    return track.album_object






















