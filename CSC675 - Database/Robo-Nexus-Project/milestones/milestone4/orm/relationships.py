class Relationship:
    def __init__(self, model_name, foreign_key, backreference=None, lazy_load=True):
        self.model_name = model_name
        self.fk = foreign_key
        self.lazy_load = lazy_load
        self.attribute_name = None
        self.backreference = backreference

    def __set_name__(self, owner, name):
        self.attribute_name = name

    def _append_backref(self, related, source):
        if not self.backreference or related is None:
            return
        if not hasattr(related, self.backreference):
            setattr(related, self.backreference, [])
        bucket = getattr(related, self.backreference)
        if source not in bucket:
            bucket.append(source)

    def __get__(self, instance, owner):
        if instance is None:
            return self

        cache = Cache(self.attribute_name)

        if self.lazy_load and cache.has(instance):
            model_object = cache.get(instance)
            self._append_backref(model_object, instance)
            return model_object

        model = owner.resolve_model(self.model_name)
        if model is None:
            return None

        fk_value = getattr(instance, self.fk, None)
        if fk_value is None:
            return None

        pk_name = model.primary_key()
        model_object = model.get(**{pk_name: fk_value})

        if self.lazy_load:
            cache.add(instance, model_object)

        self._append_backref(model_object, instance)

        return model_object


class Cache:
    def __init__(self, attribute_name):
        self.cache_name = f"_{attribute_name}_cache"

    def add(self, instance, model_object):
        setattr(instance, self.cache_name, model_object)

    def get(self, instance):
        return getattr(instance, self.cache_name)

    def has(self, instance):
        return hasattr(instance, self.cache_name)