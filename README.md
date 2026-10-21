# mammo_G4MSBG

**Monoenergetic x-ray internal breast dosimetry**, 20 keV x-rays on a breast
phantom (G4MSBG anatomical model), validated against measured air-kerma
values. Built on the Geant4-based Multi-Scale Breast Geometry (G4MSBG)
framework.

---

## 🔬 What this test does

A 20 keV mono-energetic x-ray source irradiates a breast phantom; the dose
is scored at several depths (0-4 cm, `BreastPhantomInferior`/
`BreastPhantomSuperior` regions) and the Monte Carlo air-kerma is compared
to the experimental reference value (`exp_air_kerma = 13.746 mGy` in
`parser.py`).

## 📚 Reference

- Source: `geant-validation-tests/src/mammo_G4MSBG`.
- See also `geant-config-generator/G4MSBG.md` for background on the G4MSBG
  anatomical breast model.

## Import notes

- `main.cc`, `include/`, `src/` come from `mammo_G4MSBG.cc`/`include`/`src`.
- `macro/` contains the two visualization macros found directly in that
  source tree (kept for reference, not used by CI), plus 4 concrete macros
  generated from `geant-config-generator`'s own `template.conf`/`params.conf`
  — see `validation/README.md`.
- `validation/parser.py` was **rewritten** for the `metadata()`/`parse()`
  module API that `ci-workflows/validation/export.py` actually calls (the
  first version, copied unmodified from
  `geant-config-generator/tests/geant4/mammo_G4MSBG/parser.py`, used the old
  `gts.BaseParser` class API and would have failed CI the same way
  `fanoCavity` initially did). See `validation/README.md` for details. The
  vendored `gts` package is no longer needed and was removed.
- No standalone `README`/`History` file was present alongside the C++ source
  in `geant-validation-tests/src/mammo_G4MSBG`; this summary is derived from
  `params.conf` and `parser.py` instead.
