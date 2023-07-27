import os
import sys

# Importing path
_filedir = os.path.dirname(__file__)
_rootdir = os.path.abspath(os.path.join(_filedir, ".."))
_srcdir = os.path.abspath(os.path.join(_filedir, "../src"))
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
    "soil_simulator" : ("../src", ["soil_dynamics.hpp", "soil_dynamics.cpp"])
}

templates_path = ["_templates"]
html_theme = "sphinx_rtd_theme"

# Whitelist pattern for tags and branches used for multi-versioning
smv_tag_whitelist = r"^v\d+\.\d+\.\d+$"
smv_branch_whitelist = r"^(main)$"
