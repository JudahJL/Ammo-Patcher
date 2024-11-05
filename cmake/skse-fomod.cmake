set(FOMOD_INFO_DATA 
"<fomod> \n"
"	<Name>${PROJECT_NAME}</Name> \n"
"	<Author>${PROJECT_AUTHOR}</Author> \n"
"	<Version>${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.3</Version> \n"
"	<Website>https://www.nexusmods.com/skyrimspecialedition/mods/109061/</Website> \n"
"	<Description>\n"
"A simple SKSE plugin to patch All AMMO.\n"
"You can change the speed,gravity of arrows and bolts separately.\n"
"You can also limit the speed of arrows and/or bolts between a maximum and a minimum value.\n"
"You can also optionally have practically infinite AMMO for player or followers or both.(adds one arrow or bolt when shot, you will get a notification)\n</Description> \n"
"	<Groups>\n"
"		<element>Ammo</element>\n"
"	</Groups> \n" 
"</fomod> \n"
)

if( NOT DEFINED DISTRIBUTION_DIR )
	message(FATAL_ERROR "Variable ZIP_DIR is not Defined")
endif()

if(NOT DEFINED ENV{GITHUB_ENV})
	message(STATUS "GitHub environment not detected. Executing fomod xml file modification.")
    file(WRITE "${DISTRIBUTION_DIR}/fomod/info.xml" ${FOMOD_INFO_DATA})
else()
	message(STATUS "GitHub environment detected. Skipping fomod xml file modification.")
endif()
