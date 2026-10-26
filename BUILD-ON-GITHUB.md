# Build the Windows VST3 online — no compiler on your PC

This archive contains source and a build workflow, not a compiled plugin. The workflow has not yet run on GitHub. A successful compilation does not replace testing in Studio One.

## Easiest browser-only setup

1. Sign in at https://github.com and create a new repository named `ScreechForge`. Choose Private and enable Add a README so the repository starts with a default branch.
2. Extract this archive on your PC. Open the extracted `ScreechForge` folder.
3. In the GitHub repository, choose Add file > Upload files. Drag the CONTENTS of the extracted folder into the upload area: `CMakeLists.txt`, `Source`, `tests`, and the other visible files. Do not upload the zip or an outer ScreechForge folder. Commit changes. CMakeLists.txt must appear at the repository's top level.
4. Browser uploads may omit the hidden `.github` folder. Create the workflow explicitly: Add file > Create new file. Name it exactly `.github/workflows/build.yml`. Open the included `GITHUB-WORKFLOW.txt` on your PC, copy ALL its contents and paste them into the GitHub editor. Commit changes to the default branch.
5. Open Actions > Build Windows VST3. It starts automatically on that commit; you can also choose Run workflow to start a build manually. If GitHub asks to enable Actions, enable them for this repository.
6. When the build turns green, open that run and scroll to Artifacts. Download `ScreechForge-Windows-VST3` while signed in. Artifacts are retained for 14 days; rerun the workflow if it expires.
7. Extract the downloaded zip. Copy the whole `ScreechForge.vst3` folder into `C:\Program Files\Common Files\VST3`. Restart/rescan Studio One and add ScreechForge from Instruments.

If a run turns red, open its failed step and send the error text back for troubleshooting. Do not send account credentials or tokens.

GitHub Actions availability and included usage depend on your account/plan; review GitHub's billing settings before enabling paid usage.

## If you use Git instead

Push the contents of this folder, including `.github/workflows/build.yml`, to the root of a repository. No manual workflow copy is then needed.

## References

- https://docs.github.com/en/actions/how-tos/manage-workflow-runs
- https://docs.github.com/en/actions/how-tos/manage-workflow-runs/download-workflow-artifacts
- https://github.com/actions/upload-artifact
