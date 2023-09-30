import os
import sys

# Importing path
_filedir = os.path.dirname(__file__)
_rootdir = os.path.abspath(os.path.join(_filedir, ".."))
_srcdir = os.path.abspath(os.path.join(_filedir, "../soil_simulator"))
sys.path.insert(0, _rootdir)
sys.path.insert(0, _srcdir)

project = "soil_simulator"
version = "0.0.1"
copyright = "2023, Vilella Kenny"
author = "Kenny Vilella"

extensions = [
    "breathe",
    "sphinx_multiversion",
]

breathe_projects_source = {
    "soil_simulator" : ("../", 
        ["soil_simulator/soil_dynamics.hpp",
        "soil_simulator/soil_dynamics.cpp",
        "soil_simulator/types.hpp",
        "soil_simulator/types.cpp",
        "soil_simulator/bucket_pos.hpp",
        "soil_simulator/bucket_pos.cpp",
        "soil_simulator/body_soil.hpp",
        "soil_simulator/body_soil.cpp",
        "soil_simulator/intersecting_cells.hpp",
        "soil_simulator/intersecting_cells.cpp",
        "soil_simulator/relax.hpp",
        "soil_simulator/relax.cpp",
        "soil_simulator/utils.hpp",
        "soil_simulator/utils.cpp",
        "test/example/soil_evolution.hpp",
        "test/example/soil_evolution.cpp"])
}

templates_path = ["_templates"]
html_theme = "sphinx_rtd_theme"

# Whitelist pattern for tags and branches used for multi-versioning
smv_tag_whitelist = r"^v\d+\.\d+\.\d+$"
smv_branch_whitelist = r"^(main)$"
smv_remote_whitelist = r'^(origin|upstream)$'
