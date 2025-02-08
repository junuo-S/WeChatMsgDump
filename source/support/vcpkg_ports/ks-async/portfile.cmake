vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO kingsoft-wps/ks-async
    REF 01cace949ae99269b50041b63ba37582aac7cfdf
	SHA512 19dc97b7236c7108ff23bad6b011f9a7bdee6ef2afd65f20022f74fc4bd04ff9c8ab9e8284b194f0cb2a8875907fb3948eabf5c2e1277b494984a0313d469d5f
	HEAD_REF main
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_install_cmake()

vcpkg_copy_pdbs()
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)