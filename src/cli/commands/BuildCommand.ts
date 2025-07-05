import {CommandModule} from "yargs";
import fs from 'node:fs';
import path from 'node:path';
import {exec} from "node:child_process";

export type BuildCommandArgs = {
    buildType: string;
}

// Function to copy files from source directory to destination directory
function copyFilesFromDirToDir(sourceDir: string, destDir: string) {
    // Create destination directory if it doesn't exist
    if (!fs.existsSync(destDir)) {
        fs.mkdirSync(destDir, { recursive: true });
    }

    // Read all files from source directory
    const files = fs.readdirSync(sourceDir);

    // Copy each file to destination directory
    for (const file of files) {
        const sourcePath = path.join(sourceDir, file);
        const destPath = path.join(destDir, file);

        // Check if it's a file or directory
        const stats = fs.statSync(sourcePath);

        if (stats.isFile()) {
            // Copy file
            fs.copyFileSync(sourcePath, destPath);
            console.log(`Copied ${sourcePath} to ${destPath}`);
        } else if (stats.isDirectory()) {
            // Recursively copy directory
            copyFilesFromDirToDir(sourcePath, destPath);
        }
    }
}

export const BuildCommand: CommandModule<object, BuildCommandArgs> = {
    command: 'build',
    aliases: ['b'],
    describe: 'Build the project',
    builder: (yargs) => yargs.option('buildType', {type: 'string', default: 'debug'}),
    handler: (args) => {
        const currentDir = process.cwd();
        const buildType = args.buildType.toLowerCase();
        const isDebug = buildType === 'debug';

        // Build command based on build type
        const cmakeBuildCmd = isDebug 
            ? `npx cmake-js build -l verbose -D -d ${currentDir} -O ${currentDir}/dist`
            : `npx cmake-js build -l verbose -d ${currentDir} -O ${currentDir}/dist`;

        exec(cmakeBuildCmd, (error, stdout) => {
            if (error) {
                console.error(`CMake configuration error: ${error.message}`);
                return;
            }
            console.log(`CMake build output:\n${stdout}`);

            // After successful build, copy files from Debug/Release to bin
            const buildFolder = isDebug ? 'Debug' : 'Release';
            const sourceDir = path.join(currentDir, 'dist', buildFolder);
            const destDir = path.join(currentDir, 'dist', 'bin');

            if (fs.existsSync(sourceDir)) {
                console.log(`Copying files from ${sourceDir} to ${destDir}`);
                copyFilesFromDirToDir(sourceDir, destDir);
            } else {
                console.error(`Source directory ${sourceDir} does not exist`);
            }
        });
    }
}
