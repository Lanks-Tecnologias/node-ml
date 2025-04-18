import {CommandModule} from "yargs";
import fs from 'node:fs';
import {exec} from "node:child_process";

export type BuildCommandArgs = {
    buildType: string;
}

export const BuildCommand: CommandModule<object, BuildCommandArgs> = {
    command: 'build',
    aliases: ['b'],
    describe: 'Build the project',
    builder: (yargs) => yargs.option('buildType', {type: 'string', default: 'debug'}),
    handler: () => {
        const currentDir = process.cwd();
        //const cmakeBuildRelease = `npx cmake-js build -l verbose -d ${currentDir}/ml -O ${currentDir}/ml/build`;
        const cmakeBuildDebug = `npx cmake-js build -l verbose -D -d ${currentDir}/ml -O ${currentDir}/ml/build`;
        

//         exec(cmakeBuildRelease, (error, stdout) => {
//             if (error) {
//                 console.error(`CMake configuration error: ${error.message}`);
//                 return;
//             }
//             console.log(`CMake build output:\n${stdout}`);
//         });

        exec(cmakeBuildDebug, (error, stdout) => {
            if (error) {
                console.error(`CMake configuration error: ${error.message}`);
                return;
            }
            console.log(`CMake build output:\n${stdout}`);
        });
    }
}