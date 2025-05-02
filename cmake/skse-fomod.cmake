if (NOT DEFINED PROJECT_AUTHOR)
    message(FATAL_ERROR "Variable PROJECT_AUTHOR is not Defined")
endif ()

set(FOMOD_INFO_DATA
        "<fomod> \n"
        "	<Name>${PROJECT_NAME}</Name> \n"
        "	<Author>${PROJECT_AUTHOR}</Author> \n"
        "	<Version>${CMAKE_PROJECT_VERSION}</Version> \n"
        "	<Website>https://www.nexusmods.com/skyrimspecialedition/mods/109061</Website> \n"
        "	<Description>A simple SKSE plugin to patch All AMMO.</Description>\n"
        "	<Groups>\n"
        "		<element>Ammo</element>\n"
        "	</Groups> \n"
        "</fomod> \n"
)

set(FOMOD_MODULE_CONFIG
        "<!-- Created with FOMOD Creation Tool 1.7.0.37 [http://www.nexusmods.com/fallout4/mods/6821] -->\n"
        "<config xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://qconsulting.ca/fo3/ModConfig5.0.xsd\">\n"
        "	<moduleName>${PROJECT_NAME}</moduleName> \n"
        "	<moduleImage path=\"Images\\AmmoPatcherLogo.png\" /> \n"
        "	<requiredInstallFiles> \n"
        "		<file source=\"Common\\Ammo_Patcher.json\" destination=\"SKSE\\Plugins\\Ammo_Patcher.json\" />\n"
        "		<file source=\"Common\\APConfig_schema.json\" destination=\"SKSE\\Plugins\\APConfig_schema.json\" />\n"
        "		<file source=\"Common\\Action Based Projectiles.json\" destination=\"SKSE\\Plugins\\Ammo Patcher\\Presets\\Action Based Projectiles.json\" />\n"
        "		<file source=\"Common\\Default.json\" destination=\"SKSE\\Plugins\\Ammo Patcher\\Presets\\Default.json\" />\n"
        "		<file source=\"Common\\Vanilla.json\" destination=\"SKSE\\Plugins\\Ammo Patcher\\Presets\\Vanilla.json\" />\n"
        "	</requiredInstallFiles> \n"
        "	<installSteps order=\"Explicit\"> \n"
        "		<installStep name=\"Developer Options\"> \n"
        "			<optionalFileGroups order=\"Explicit\"> \n"
        "				<group name=\"Build Type\" type=\"SelectExactlyOne\">\n"
        "					<plugins order=\"Explicit\"> \n"
        "						<plugin name=\"Release\"> \n"
        "							<description>Installs the Release Build: Highly recommended for most users, offering the highest performance.</description>\n"
        "							<image path=\"Images\\AmmoPatcherLogo.png\" /> \n"
        "							<conditionFlags> \n"
        "								<flag name=\"release\">On</flag>\n"
        "							</conditionFlags>\n"
        "							<files> \n"
        "								<file source=\"Plugin - Release\\${PROJECT_NAME}.dll\" destination=\"SKSE\\Plugins\\${PROJECT_NAME}.dll\"/>\n"
        "							</files> \n"
        "							<typeDescriptor> \n"
        "								<type name=\"Recommended\"/>\n"
        "							</typeDescriptor>\n"
        "						</plugin> \n"
        "						<plugin name=\"Debug\"> \n"
        "							<description>Installs the Debug Build: Ideal for development and testing.</description> \n"
        "							<image path=\"Images\\AmmoPatcherLogo.png\" /> \n"
        "							<conditionFlags> \n"
        "								<flag name=\"debug\">On</flag>\n"
        "							</conditionFlags>\n"
        "							<files> \n"
        "								<file source=\"Plugin - Debug\\${PROJECT_NAME}.dll\" destination=\"SKSE\\Plugins\\${PROJECT_NAME}.dll\"/> \n"
        "								<file source=\"Plugin - Debug\\${PROJECT_NAME}.pdb\" destination=\"SKSE\\Plugins\\${PROJECT_NAME}.pdb\"/>\n"
        "							</files> \n"
        "							<typeDescriptor> \n"
        "								<type name=\"Optional\"/>\n"
        "							</typeDescriptor>\n"
        "						</plugin> \n"
        "					</plugins>\n"
        "				</group> \n"
        "			</optionalFileGroups>\n"
        "		</installStep> \n"
        "	</installSteps>\n"
        "</config>\n"
        "\n"
)

if (NOT DEFINED DISTRIBUTION_DIR)
    message(FATAL_ERROR "Variable ZIP_DIR is not Defined")
endif ()

message(STATUS "Executing fomod xml file modification.")
file(WRITE "${DISTRIBUTION_DIR}/fomod/info.xml" ${FOMOD_INFO_DATA})
file(WRITE "${DISTRIBUTION_DIR}/fomod/ModuleConfig.xml" ${FOMOD_MODULE_CONFIG})
