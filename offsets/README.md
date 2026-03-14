# CS2 Offsets (cs2-dumper output)

This folder contains generated headers from [a2x/cs2-dumper](https://github.com/a2x/cs2-dumper).

- **output/** — latest dump (run the dumper to refresh after game updates).
- **client_dll.hpp**, **offsets.hpp**, etc. — schema and static offsets.

The Litware cheat uses **runtime schema** (SchemaSystem) for class/field offsets and **pattern scanning** in `interfaces.cpp` for globals. After a CS2 update:

1. Re-run cs2-dumper and replace contents of `output/`.
2. If the game breaks, update patterns in `aporia-last/src/core/interfaces/interfaces.cpp` (search for `g_modules->m_client.find(...)` etc.) using new signatures from the dumper or community.
