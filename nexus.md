# ⚔️ Ammo Patcher — Take Control of Every Shot  

**Ammo Patcher** is a lightweight SKSE plugin that lets you **customize arrows and bolts in real time**.  
Speed, gravity, damage, sound — even infinite ammo.  

No patch rebuilds. No xEdit hassle. Just launch and tweak.  

---

## ✨ Core Features
- 🎯 Fine-tune **speed**, **gravity**, **sound level**, and **damage** for all arrows & bolts.  
- ♾️ Enable **infinite ammo** for you and your followers.  
- ⚡ Apply changes at **runtime** — no patchers required.  
- 🛠️ Configure everything via JSON or in-game with **[SKSE Menu Framework v2](https://www.nexusmods.com/skyrimspecialedition/mods/120352)**.  

---

## 🔬 Advanced Options
- Exclude entire mods or individual AMMO records.  
- Randomize ammo speed for unique playthroughs.  
- Presets for quick swapping.  
- Detailed benchmarking (nanoseconds → minutes).  
- Custom logging levels (`trace` → `off`).  

---

## 🚀 Quick Start
**Requirements:**  
- [SKSE64](https://skse.silverlock.org/) (hard requirement)  
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444) (hard requirement)  
- (Optional) [SKSE Menu Framework v2](https://www.nexusmods.com/skyrimspecialedition/mods/120352) for in-game editing  

**Install:**  
1. Download from the [Files tab](https://www.nexusmods.com/skyrimspecialedition/mods/109061?tab=files).  
2. Install with MO2 or Vortex.  
3. Configure with JSON or SKSE Menu Framework.  

---

## ⚖️ Why Choose Ammo Patcher?
Compared to older solutions like **Ammo Tweaks** or **Synthesis patchers**:  
- ✅ Works at **runtime** — no need to rebuild patches.  
- ✅ Offers **finer control** with JSON configs.  
- ✅ Supports **in-game editing & benchmarking**.  

---

## 💬 Community & Contributions
Questions? Feedback? Feature ideas?  
- Reach me in the [comments](https://www.nexusmods.com/skyrimspecialedition/mods/109061?tab=posts) or on **GroundAura's Discord** (@Judah).  
- Explore the source code on [GitHub](https://github.com/JudahJL/Ammo-Patcher).  

---

## 🛠️ Technical Deep Dive

### Patching Order and Sequence

The plugin applies modifications in a specific order to ensure proper functionality:

**For Arrows:**
1. **Sound Level** → Sets the projectile's sound level
2. **Speed** → Sets the projectile velocity
3. **Gravity** → Sets the gravity multiplier
4. **Damage Limiting** → Clamps ammo damage to min/max range
5. **Speed Limiting** → Clamps projectile speed to min/max range
6. **Speed Randomization** → Randomizes speed within min/max range

**For Bolts:**
1. **Sound Level** → Sets the projectile's sound level
2. **Speed** → Sets the projectile velocity
3. **Gravity** → Sets the gravity multiplier
4. **Speed Limiting** → Clamps projectile speed to min/max range
5. **Speed Randomization** → Randomizes speed within min/max range
6. **Damage Limiting** → Clamps ammo damage to min/max range

**Important Notes:**
- **Speed Randomization** happens **after** Speed Limiting, so it can override the limits
- **Damage Limiting** is applied to the ammo record itself, not the projectile
- **Gravity** and **Speed** are applied to the projectile data
- **Sound Level** is applied to the projectile's sound properties
- All modifications are applied **only if** the respective feature is enabled

### Whitelisting/Blacklisting System

The plugin uses a sophisticated rule system in `AP_Rules.json` to control which ammo gets patched:

**Rule Priority:**
1. **Per-record rules** (highest priority) - Individual FormID overrides
2. **Global file rules** - Apply to entire mod files
3. **Default behavior** - Skip non-playable ammo (`kNonPlayable` flag)

**Rule Types:**
- `whitelist` - Records WILL be patched(skips the kNonPlayable flag check, for ammo like [Telekinesis Arrows](https://en.uesp.net/wiki/Skyrim:Arcane_Archer_Pack_Items#Telekinesis_Arrow) which was unaffected previously)
- `blacklist` - These records will be skipped
- `global` - File-wide rule for all ammo in that mod

### Configuration Structure

The plugin uses a JSON schema-validated configuration system with nested options for arrows and bolts. Each ammo type can have independent settings for:

- **Gravity Control** - Enable/disable and set gravity values
- **Speed Control** - Enable/disable, set speed, limit ranges, or randomize
- **Damage Control** - Enable/disable and set damage limits
- **Sound Control** - Enable/disable and set sound levels
- **Infinite Ammo** - Enable/disable for player and teammates

### Properties Modified

The plugin directly modifies these Skyrim engine properties:

- **Projectile Speed** - Controls how fast the projectile travels
- **Projectile Gravity** - Controls gravity effects (0 = no gravity)
- **Ammo Damage** - The damage dealt by the ammo
- **Sound Level** - How loud the ammo is when fired

### Infinite Ammo Implementation

Infinite ammo works by monitoring when ammo is removed from player or follower inventories and automatically replenishing it. The system only activates when no game menus are open to prevent conflicts with inventory management.

### Compatibility Features

- **FormID Resolution**: Handles both regular ESP and ESL (light) plugins correctly
- **Schema Validation**: All JSON configs are validated against a schema
- **Error Handling**: Graceful fallbacks for invalid configurations
- **Logging**: Comprehensive debug logging for troubleshooting

### Performance Considerations

- **Lazy Loading**: Only processes ammo when patches are enabled
- **Event Optimization**: Infinite ammo events only register when needed
- **Memory Efficient**: Uses references to existing game objects
- **Benchmarking**: Built-in timing for performance monitoring
