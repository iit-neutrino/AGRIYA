#!/bin/bash

bash ./Scripts/runDYB.sh
bash ./Scripts/runDYB_New.sh
bash ./Scripts/runGlobal.sh
bash ./Scripts/runRENO.sh

bash ./Scripts/runDYB+RENO.sh
bash ./Scripts/runDYB_ImprovedDE+RENO.sh

bash ./Scripts/runGlobal+DYB.sh
bash ./Scripts/runGlobal+DYB_New.sh
bash ./Scripts/runGlobal+RENO.sh

bash ./Scripts/runGlobal+DYB_New+RENO.sh
