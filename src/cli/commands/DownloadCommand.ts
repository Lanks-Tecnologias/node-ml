import {CommandModule} from "yargs";
import { arch } from 'process';
// import fs from "fs-extra";
import chalk from "chalk";
import {BuildGpu, nodeLlamaCppGpuOptions, parseNodeLlamaCppGpuOption} from "../../types.js";

type DownloadCommandArgs = {
    repo?: string,
    release?: "latest" | string,
    arch?: typeof arch,
    nodeTarget?: string,
    gpu?: BuildGpu | "auto",
    skipBuild?: boolean,
    noBundle?: boolean,
    noUsageExample?: boolean,

    /** @internal */
    updateBinariesReleaseMetadataAndSaveGitBundle?: boolean
}

export const DownloadCommand: CommandModule<object, DownloadCommandArgs> = {
    command: "download",
    describe: "Download a release of a package and compile it",
    aliases: ["d"],
    builder(yargs) {
        return yargs
            .option("repo", {
                type: "string",
                default: "ggml-org/ggml",
                description: "The GitHub repository to download a release."
            })
            .option("release", {
                type: "string",
                default: 'latest',
                description: "The tag of the llama.cpp release to download. Set to `latest` to download the latest release. Can also be set via the `NODE_LLAMA_CPP_REPO_RELEASE` environment variable"
            })
            .option("arch", {
                alias: "a",
                type: "string",
                coerce: (value) => value,
                description: "The architecture to compile llama.cpp for"
            })
            .option("nodeTarget", {
                alias: "t",
                type: "string",
                description: "The Node.js version to compile llama.cpp for. Example: `v18.0.0`"
            })
            .option("gpu", {
                type: "string",
                default: 'auto',

                // yargs types don't support passing `false` as a choice, although it is supported by yargs
                choices: nodeLlamaCppGpuOptions as any as Exclude<typeof nodeLlamaCppGpuOptions[number], false>[],
                coerce: parseNodeLlamaCppGpuOption,
                description: "Compute layer implementation type to use for llama.cpp"
            })
            .option("skipBuild", {
                alias: "sb",
                type: "boolean",
                default: false,
                description: "Skip building llama.cpp after downloading it"
            })
            .option("noBundle", {
                alias: "nb",
                type: "boolean",
                default: false,
                description: "Download a llama.cpp release only from GitHub, even if a local git bundle exists for the release"
            })
            .option("noUsageExample", {
                alias: "nu",
                type: "boolean",
                default: false,
                description: "Don't print code usage example after building"
            })
            .option("updateBinariesReleaseMetadataAndSaveGitBundle", {
                type: "boolean",
                hidden: true, // this is only for the CI to use
                default: false,
                description: "Update the binariesGithubRelease.json file with the release of llama.cpp that was downloaded"
            });
    },
    handler: (args) => {
        const {
            repo ,
            release ,
            arch = undefined,
            nodeTarget = undefined,
            gpu ,
            skipBuild = false,
            noBundle = false,
            noUsageExample = false,

            updateBinariesReleaseMetadataAndSaveGitBundle = false
        } = args;

        const [githubOwner, githubRepo] = repo!.split("/");
        if (githubOwner == null || githubRepo == null)
            throw new Error(`Invalid GitHub repository: ${repo}`);
        
        console.log(`${chalk.yellow("Repo:")} ${repo}`);
        console.log(`${chalk.yellow("Release:")} ${release}`);
        console.log(args);
    }
}