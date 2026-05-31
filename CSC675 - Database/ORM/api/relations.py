"""
    This file will implement the base code to create relationships between tables,
    For example, getting all the reviews' model instances from a specific product model

    Note: we will implement this file in class in the ORM demo lecture

"""
class Relation:
    """
    track7 = Track.get(id=7)
    album = track7.album_object # returns album_id = 5 instance
    """
    def __init__(self, model_name, foreign_key, backreference=None, lazy=True):
        """

        :param model_name: 'Album'
        :param foreign_key: 'album_id'
        """
        self.model_name = model_name
        self.fk = foreign_key
        self.backreference = backreference
        self.lazy = lazy
        self.attribute_name = None



    def __set_name__(self, owner, name):
        """

        :param owner: Track class
        :param name: album_object (not the instance, the name of the attribute)
        :return:
        """
        self.attribute_name = name


    def __get__(self, instance, owner):
        """
        Will override the returned object from the constructor
        it will not return Relation(..) anymore unless the instance is Null
        :param instance: track object that we create from calling track = Track.get(..)
        :param owner: Track class
        :return: album object
        """
        if instance is None: # track = Track.get(id=100) doesnt exists in DB
            return self

        # Check if the model was already cached
        cache = None
        if self.lazy:
            cache = Cache(self.attribute_name)
            if cache.has_model(instance):
                return cache.get_model(instance) # album instance


        # Album class based on the based on model name 'Album' -> Album

        # Track.resolve_model('Album') --> Album class
        model = owner.resolve_model(self.model_name)
        # go to the __dict__ and get the FK value from the album_id fk name
        pk_value = getattr(instance, self.fk)
        # we know which class we need to create an instance object from.
        # album = Album.get(id=5)

        if model is None and pk_value is None:
            return None

        # album = Album.get(id=5)
        obj_model = model.get(id=pk_value)

        # we know that the album was not in the cache, then
        # we need to add the album to the cache
        if self.lazy and cache is not None:
            cache.add_model(instance, obj_model)

        # set backreference to track
        # track7 = album.track
        setattr(obj_model, self.backreference, instance)
        return obj_model # album instance of the class Album where album_id = 5

class Cache:
    """
    album_object = Relation(......)
    """
    def __init__(self, attribute_name ):
        self.cache_name = f"_{attribute_name}_cache"

    def add_model(self, instance, object):
        setattr(instance, self.cache_name, object)

    def get_model(self, instance):
        return getattr(instance, self.cache_name)

    def has_model(self, instance):
        return hasattr(instance, self.cache_name)
























