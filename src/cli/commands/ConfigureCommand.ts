import {CommandModule} from "yargs";
import {ResetMode, simpleGit} from "simple-git";
import fs from 'node:fs';
import {exec} from "node:child_process";

export type ConfigureCommandArgs = {
    debug: boolean
}
// Constants for repository URLs
const GGML_REPO_URL = "https://github.com/ggml-org/ggml.git";
const ENCODEC_REPO_URL = "https://github.com/PABannier/encodec.cpp.git";
const BARK_REPO_URL = "https://github.com/PABannier/bark.cpp.git";
const WHISPER_REPO_URL = "https://github.com/ggml-org/whisper.cpp.git";
const LLAMA_REPO_URL = "https://github.com/ggml-org/llama.cpp.git";
const STABLE_DIFFUSION_REPO_URL = "https://github.com/leejet/stable-diffusion.cpp.git";

// Helper function to initialize repositories
async function initializeRepo(repoDir: string, repoUrl: string, branch: string = "master") {
    const repoExists = fs.existsSync(repoDir);
    const isRepo = repoExists ? await simpleGit(repoDir).checkIsRepo() : false;

    if (repoExists && isRepo) {
        console.log(`Repository at ${repoDir} is already initialized.`);
        await simpleGit(repoDir).fetch();
        await simpleGit(repoDir).pull("origin", branch);
    } else {
        console.log(`Initializing repository from ${repoUrl}`);
        await simpleGit().submoduleAdd(repoUrl, repoDir)
        //await simpleGit().clone(repoUrl, repoDir);
    }
}

// Configure GGML
async function configureGgml(currentDir: string) {
    const ggmlRepoDir = `${currentDir}/ml/ggml/ggml`;
    await initializeRepo(ggmlRepoDir, GGML_REPO_URL, "master");
}

async function applyPatchesFromDir(currentDir: string, repoDir: string) {
    const patchDir = `${currentDir}/patches`;
    const patchFiles = fs.readdirSync(patchDir);

    for (const patchFile of patchFiles) {
        const patchPath = `${patchDir}/${patchFile}`;
        console.log(`Applying patch: ${patchPath}`);
        try {
            await simpleGit(repoDir).applyPatch(patchPath, {"--reject": null, "--whitespace": "fix"});
        }catch (e) {
            console.error(e)
        }
    }
}

// Configure Encodec
async function configureEncodec(currentDir: string) {
    const encodecDir = `${currentDir}/ml/encodec`;
    const encodecRepoDir = `${encodecDir}/encodec.cpp`;
    await initializeRepo(encodecRepoDir, ENCODEC_REPO_URL, "main");

    await applyPatchesFromDir(encodecDir, encodecRepoDir);
}

async function configureBark(currentDir: string) {
    const barkDir = `${currentDir}/ml/bark`;
    const repoDir = `${barkDir}/bark.cpp`;
    await initializeRepo(repoDir, BARK_REPO_URL, "main");
    await simpleGit(repoDir).reset(ResetMode.HARD)
    await applyPatchesFromDir(barkDir, repoDir);
}

async function configureWhisper(currentDir: string) {
    const whisperDir = `${currentDir}/ml/whisper`;
    const repoDir = `${whisperDir}/whisper.cpp`;
    await initializeRepo(repoDir, WHISPER_REPO_URL, "master");
}

async function configureLlama(currentDir: string) {
    const llamaDir = `${currentDir}/ml/llama`;
    const repoDir = `${llamaDir}/llama.cpp`;
    await initializeRepo(repoDir, LLAMA_REPO_URL, "master");
}

async function configureStableDiffusion(currentDir: string) {
    const stableDiffusionDir = `${currentDir}/ml/stable-diffusion`;
    const repoDir = `${stableDiffusionDir}/stable-diffusion.cpp`;
    await initializeRepo(repoDir, STABLE_DIFFUSION_REPO_URL, "master");
    await applyPatchesFromDir(stableDiffusionDir, repoDir);
}

export const ConfigureCommand: CommandModule<object, ConfigureCommandArgs> = {
    command: 'configure',
    describe: 'Configure the workspace to build all packages',
    builder(yargs) {
        return yargs
            .option('debug', {
                type: "boolean",
                default: false,
                alias: ['d'],
                describe: 'Set debug build type'
            })
    },
    handler: async (args) => {
        const currentDir = process.cwd();
        console.log(`Configuring workspace in ${currentDir}`);
        if (!fs.existsSync(`${currentDir}/.gitmodules`)){
            fs.writeFileSync(`${currentDir}/.gitmodules`, '', { encoding: 'utf8' });
        }
        // Configure all repositories
        await configureGgml(currentDir);
        await configureEncodec(currentDir);
        await configureBark(currentDir);
        await configureWhisper(currentDir);
        await configureLlama(currentDir);
        await configureStableDiffusion(currentDir);

        // Run cmake-js configuration

        if (args.debug){
            const cmakeConfigureDebug = `npx cmake-js configure -D -d ${currentDir}/ml -O ${currentDir}/ml/build`;

            exec(cmakeConfigureDebug, (error, stdout) => {
                if (error) {
                    console.error(`CMake configuration error: ${error.message}`);
                    return;
                }
                console.log(`CMake configuration output:\n${stdout}`);
            });

        } else {
            const cmakeConfigureRelease = `npx cmake-js configure -d ${currentDir}/ml -O ${currentDir}/ml/build`;

            exec(cmakeConfigureRelease, (error, stdout) => {
                if (error) {
                    console.error(`CMake configuration error: ${error.message}`);
                    return;
                }
                console.log(`CMake configuration output:\n${stdout}`);
            });
        }

    }
};