import path from "path";
const { NodeRecordingSdk } = require('../build/Release/AgoraRecorder.node')

export default NodeRecordingSdk

export const agoraBinPath = path.join(__dirname, '../sdk/bin/')
