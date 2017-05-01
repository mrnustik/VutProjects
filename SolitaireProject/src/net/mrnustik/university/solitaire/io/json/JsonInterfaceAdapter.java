package net.mrnustik.university.solitaire.io.json;

import com.google.gson.*;

import java.lang.reflect.Type;

/**
 * Class to be used with Google JSON library (GSON).
 * This class allows to save Objects represented by Interfaces and then load them back again.
 * When serializing, it interrupts serializing process and adds class metadata to the file.
 * When deserializing, it loads the metadata stored before and creates actual classes based on them.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public final class JsonInterfaceAdapter<T> implements JsonSerializer<T>, JsonDeserializer<T> {

    /**
     * Method that is used to add class metadata to the output json element for T type.
     * @param object to be serialized
     * @param interfaceType type of the interface used to describe T
     * @param context of the serialization
     * @return JSON element with metadata inside it
     */
    public JsonElement serialize(T object, Type interfaceType, JsonSerializationContext context) {
        final JsonObject wrapper = new JsonObject();
        wrapper.addProperty("type", object.getClass().getName());
        wrapper.add("data", context.serialize(object));
        return wrapper;
    }

    /**
     * Method for deserializing the interface into proper from JSON element.
     * @param elem wrapper element with metadata inside
     * @param interfaceType type of the T class
     * @param context deserialization context
     * @return class that implements T interface
     * @throws JsonParseException when class could not be found for the specified type
     */
    public T deserialize(JsonElement elem, Type interfaceType, JsonDeserializationContext context) throws JsonParseException {
        final JsonObject wrapper = (JsonObject) elem;
        final JsonElement typeName = get(wrapper, "type");
        final JsonElement data = get(wrapper, "data");
        final Type actualType = typeForName(typeName);
        return context.deserialize(data, actualType);
    }

    /**
     * Method used for retrieving Type of class based on its name
     * @param typeElem JSON string element representing the class name
     * @return type of class defined by name in typeElem
     */
    private Type typeForName(final JsonElement typeElem) {
        try {
            return Class.forName(typeElem.getAsString());
        } catch (ClassNotFoundException e) {
            throw new JsonParseException(e);
        }
    }

    /**
     * Method used for wrapping getting data from JSONObject
     * @param wrapper JSON object to be retrieved from
     * @param memberName name of the attribute to be retrieved
     * @return JSON element found in wrapper as an attribute under memberName
     */
    private JsonElement get(final JsonObject wrapper, String memberName) {
        final JsonElement elem = wrapper.get(memberName);
        if (elem == null)
            throw new JsonParseException("no '" + memberName + "' member found in what was expected to be an interface wrapper");
        return elem;
    }
}