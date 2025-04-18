import { createRequire } from "module";
const require = createRequire(import.meta.url);
const ml = require("../ml/build/Debug/ml.node");

ml.loadAllBackends()
debugger
console.log(`${ml.getDeviceCount()} devices found`);
