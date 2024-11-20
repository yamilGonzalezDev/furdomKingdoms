#include "entity.hpp"

void initBody(b2Body* body, Kind entityKind, void* object)
{
    UserdataTag* tag = new UserdataTag{entityKind, object};

    b2BodyUserData& ud = body->GetUserData();

    ud.pointer = reinterpret_cast<uintptr_t>(tag);
}
