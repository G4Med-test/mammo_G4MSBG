#!/usr/bin/env python

# mammo_G4MSBG portal parser: metadata() reads the macro, parse() the
# captured stdout (dose deposited in each of the 30 TLD positions, scaled
# to the experimental air-kerma reference).
#
# Ported from geant-config-generator/tests/geant4/mammo_G4MSBG/parser.py
# (gts.BaseParser) to the module-level metadata()/parse() API expected by
# ci-workflows/validation/export.py.
import os

from geantval import getJSON, one_command, single_run

# Particle (gamma/"x-ray") and beam energy (20 keV) are hardcoded in
# PrimaryGeneratorAction.cc, not configurable from the macro.
BEAM_PARTICLE = "x-ray"
BEAM_ENERGY_KEV = 20.0
EXP_AIR_KERMA_MGY = 13.746
# Ratio of mass energy-absorption coefficients at 20 keV for dry air and the
# TLD material (NIST database), unchanged from the original parser.
AIR_TO_TLD_COEF = 0.808


def metadata(commands):
    single_run(commands)
    superior = one_command(commands, "/app/breast/setBreastPhantom_Superior")
    return {"TEST": "mammo_G4MSBG",
            "PHYSLIST": one_command(commands, "/testem/phys/addPhysics"),
            "INFERIOR": one_command(commands, "/app/breast/setBreastPhantom_Inferior"),
            "SUPERIOR": superior}


def read_stdout(filename):
    x, y, yerr = [], [], []
    ratio_kerma_exp_mc = None
    last_y = None
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if "Reference Air-Kerma" in line:
                mc_air_kerma_gy = float(line.split()[-2])
                ratio_kerma_exp_mc = EXP_AIR_KERMA_MGY / (mc_air_kerma_gy * 1000.)
                continue
            if "Dose Deposited" not in line:
                continue
            if ratio_kerma_exp_mc is None:
                raise ValueError("Dose line found before Reference Air-Kerma: " + line)
            if "Uncertainty" not in line:
                fields = line.split()
                last_y = float(fields[6]) * 1000. * ratio_kerma_exp_mc * AIR_TO_TLD_COEF
                y.append(last_y)
                x.append(int(fields[5].replace(":", "")))
            elif "%" in line:
                er_rel = float(line.split()[7])
                yerr.append(er_rel * last_y / 100.)
    if not x:
        raise ValueError("No TLD dose lines found in " + filename)
    return x, y, yerr


def parse(job):
    x, y, yerr = read_stdout(os.path.join(job["path"], "test_stdout.txt"))
    rjson = getJSON(job, "chart",
                     mctool_name="GEANT4",
                     mctool_model=job["PHYSLIST"],
                     observableName="dose deposition",
                     targetName="TLD",
                     beamParticle=BEAM_PARTICLE,
                     beamEnergies=[BEAM_ENERGY_KEV],
                     title="Dose Deposited in TLD",
                     xAxisName="TLD position",
                     yAxisName="dose (mGy)",
                     xValues=x,
                     yValues=y,
                     yStatErrorsPlus=yerr,
                     yStatErrorsMinus=yerr,
                     parameters=[{"names": "BreastPhantomSuperior", "values": job["SUPERIOR"]}])
    yield rjson
