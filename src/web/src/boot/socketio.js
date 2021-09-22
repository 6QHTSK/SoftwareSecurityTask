import { boot } from 'quasar/wrappers'
// import VueSocketIO from "vue-socket.io";
import io from 'socket.io-client'


// "async" is optional;
// more info on params: https://v2.quasar.dev/quasar-cli/boot-files
export default boot(async ({ app }) => {
  app.config.globalProperties.$socketIO = null
  app.config.globalProperties.$socketIO = await io('http://localhost:9013')
})

export {io}
