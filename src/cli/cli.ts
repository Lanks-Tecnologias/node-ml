import yargs from "yargs"
import {DownloadCommand} from "./commands/DownloadCommand.js";
import {hideBin} from "yargs/helpers";
import {ConfigureCommand} from "./commands/ConfigureCommand.js";

const yarg =  yargs(hideBin(process.argv));

yarg
    .command(ConfigureCommand)
    .command(DownloadCommand)
    .recommendCommands()
    .demandCommand(1)
    .strict()
    .strictCommands()
    .alias("v", "version")
    .help("h")
    .alias("h", "help")
    .version("0.0.0")
    .wrap(Math.min(130, yarg.terminalWidth()))
    .parse();