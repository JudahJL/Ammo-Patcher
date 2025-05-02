import argparse
import json
from pathlib import Path
from typing import Literal, Annotated

from pydantic import BaseModel, ConfigDict, Field


class LogLevelModel(BaseModel):
    model_config = ConfigDict(extra="forbid")
    LogLevel: Literal["trace", "debug", "info", "warning", "error", "critical", "off"]


type SoundLevelType = Literal["kLoud", "kNormal", "kSilent", "kVeryLoud", "kQuiet"]

type EnableType = bool
type GravityType = float
type SpeedType = float
type MinType = float
type MaxType = float


class SoundModel(BaseModel):
    class ChangeSoundLevelModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        SoundLevel: Annotated[SoundLevelType, Field(alias="Sound Level")]

    model_config = ConfigDict(extra="forbid")

    ChangeSoundLevel: Annotated[ChangeSoundLevelModel, Field(alias="Change Sound Level")]


class InfiniteAmmoModel(BaseModel):
    model_config = ConfigDict(extra="forbid")
    Player: bool
    Teammate: bool


class ArrowModel(BaseModel):
    class ChangeGravityModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Gravity: GravityType

    class ChangeSpeedModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Speed: SpeedType

    class LimitSpeedModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Min: MinType
        Max: MaxType

    class RandomizeSpeedModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Min: MinType
        Max: MaxType

    class LimitDamageModel(BaseModel):
        model_config = ConfigDict(extra="forbid")

        Enable: EnableType
        Min: MinType
        Max: MaxType

    model_config = ConfigDict(extra="forbid")

    EnableArrowPatch: Annotated[bool, Field(alias="Enable Arrow Patch")]
    ChangeGravity: Annotated[ChangeGravityModel, Field(alias="Change Gravity")]
    ChangeSpeed: Annotated[ChangeSpeedModel, Field(alias="Change Speed")]
    LimitSpeed: Annotated[LimitSpeedModel, Field(alias="Limit Speed")]
    RandomizeSpeed: Annotated[RandomizeSpeedModel, Field(alias="Randomize Speed")]
    LimitDamage: Annotated[LimitDamageModel, Field(alias="Limit Damage")]
    Sound: SoundModel


class BoltModel(BaseModel):
    class ChangeGravityModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Gravity: GravityType

    class ChangeSpeedModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Speed: SpeedType

    class LimitSpeedModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Min: MinType
        Max: MaxType

    class RandomizeSpeedModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Min: MinType
        Max: MaxType

    class LimitDamageModel(BaseModel):
        model_config = ConfigDict(extra="forbid")
        Enable: EnableType
        Min: MinType
        Max: MaxType

    model_config = ConfigDict(extra="forbid")

    EnableBoltPatch: Annotated[bool, Field(alias="Enable Bolt Patch")]
    ChangeGravity: Annotated[ChangeGravityModel, Field(alias="Change Gravity")]
    ChangeSpeed: Annotated[ChangeSpeedModel, Field(alias="Change Speed")]
    LimitSpeed: Annotated[LimitSpeedModel, Field(alias="Limit Speed")]
    RandomizeSpeed: Annotated[RandomizeSpeedModel, Field(alias="Randomize Speed")]
    LimitDamage: Annotated[LimitDamageModel, Field(alias="Limit Damage")]
    Sound: SoundModel


class AmmoModel(BaseModel):
    model_config = ConfigDict(extra="forbid")
    InfiniteAMMO: Annotated[InfiniteAmmoModel, Field(alias="Infinite AMMO")]
    Arrow: ArrowModel
    Bolt: BoltModel


class APModel(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra="forbid", strict=True, validate_assignment=True,
                              validate_default=True, regex_engine='python-re')
    Logging: LogLevelModel
    AMMO: AmmoModel


def main(using_rapidjson: bool):
    base_dir = Path(__file__).parent.parent.resolve()
    main_config_dir = base_dir / "main config"
    files = [
        main_config_dir / "Default.json",
        main_config_dir / "Vanilla.json",
        main_config_dir / "Action Based Projectiles.json",
    ]

    for file in files:
        if file.exists():
            with open(file, 'r') as f:
                try:
                    APModel.model_validate_json(strict=True, json_data=f.read())
                except Exception as e:
                    print(type(e).__name__, str(e))

    if schema := base_dir / "main config" / "APConfig_schema.json":
        with open(schema, 'w') as f:
            # noinspection PyRedundantParentheses
            if using_rapidjson:  # rapidjson uses draft 4, so a workaround
                temp = APModel.model_json_schema(ref_template="#/definitions/{model}")
                temp["definitions"] = temp.pop("$defs")
                # noinspection PyTypeChecker
                json.dump(obj=temp, indent=2, fp=f)
            else:
                # noinspection PyTypeChecker
                json.dump(obj=APModel.model_json_schema(), indent=2, fp=f)  # this creates a draft 2020-12 schema
    del schema


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Toggle between using rapidjson(draft 4) or draft 2020-12.")
    parser.add_argument('--rapidjson', action=argparse.BooleanOptionalAction)

    args = parser.parse_args()

    main(args.rapidjson)
