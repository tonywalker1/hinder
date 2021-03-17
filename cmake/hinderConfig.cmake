################################################################################
# error checking
################################################################################
if (NOT hinder_FIND_COMPONENTS)
    set(hinder_NOT_FOUND_MESSAGE "The hinder package requires at least one component")
    set(hinder_FOUND False)
    return()
endif ()

################################################################################
# handle find_package options
################################################################################
set(hinder_required_option)
if (hinder_FIND_REQUIRED)
    set(hinder_required_option REQUIRED)
endif ()
set(hinder_quiet_option)
if (hinder_FIND_QUIET)
    set(hinder_quiet_option QUIET)
endif ()

################################################################################
# run find_package for each component
################################################################################
foreach (module ${hinder_FIND_COMPONENTS})
    message(STATUS "--> finding ${module}")
    find_package(hinder_${module}
        ${hinder_required_option}
        ${hinder_quiet_option}
        )
endforeach ()
