<template>
  <q-page class="row wrap justify-center items-start content-start">
    <div class="col-7" style="padding-top: 1em">
      <div class="fit column wrap q-gutter-sm">
        <q-banner dense inline-actions class="text-white bg-red" v-if="!socketConnect">
          <template v-slot:avatar>
            <q-icon name="signal_wifi_off"/>
          </template>
          离线状态，无法连接到行为分析器
        </q-banner>
        <q-card style="overflow: auto;">
          <q-card-section class="fit column wrap q-gutter-md">
            <q-input dense v-model="targetFile" bottom-slots :readonly="startTrace">
              <template v-slot:after>
                <div style="width: 6em; padding-left: 0.5em">
                  <q-btn v-if="!startTrace" class="full-width" color="primary" dense label="开始追踪"
                         :disable="targetFile === '' || !socketConnect"
                         @click="btnStartTrace"></q-btn>
                  <q-btn v-else color="secondary" class="full-width" dense label="停止追踪" @click="btnStopTrace"
                         :disable="!socketConnect"></q-btn>
                </div>
              </template>
              <template v-slot:hint>
                <transition-group name="hint">
                  <p v-if="runtimeInfo.pid === 0" style="transition: all 200ms;">输入需要追踪的目标文件的路径</p>
                  <div v-else class="fit row wrap q-gutter-xs items-end content-end" style="transition: all 200ms;">
                    <q-badge rounded class="q-mx-xs" :label="'PID: '+ runtimeInfo.pid"></q-badge>
                    <q-badge rounded class="q-mx-xs" :label="'ThreadID: ' + runtimeInfo.threadID"></q-badge>
                  </div>
                </transition-group>
              </template>
            </q-input>
          </q-card-section>
        </q-card>
        <q-card v-if="firstTrace">
          <q-card-section>
            <transition-group name="event" tag="q-scroll-area" class="row q-gutter-xs full-width full-height">
              <q-chip rounded style="transition:all 200ms" color="secondary" text-color="white" :key="0">追踪记录</q-chip>
              <q-chip clickable rounded v-for="i in eventCount" :key="eventCount - i" style="transition:all 200ms"
                      :color="viewEvent === eventCount - i ? 'accent': 'secondary'"
                      @click="changeViewEvent(eventCount - i)" text-color="white">#{{ eventCount - i + 1 }}
              </q-chip>
            </transition-group>
          </q-card-section>
        </q-card>
        <div class="fit row wrap justify-center" v-if="firstTrace">
          <q-card style="height: 45em;width:220px">

            <q-list bordered>

              <q-item clickable @click="filterSwitch['Main'] = !filterSwitch['Main']" :active="filterSwitch['Main']"
                      active-class="bg-blue-1 text-primary">
                <q-item-section avatar>
                  <q-avatar icon="home"></q-avatar>
                </q-item-section>

                <q-item-section>
                  <q-item-label>追踪总计</q-item-label>
                  <q-item-label caption>Total</q-item-label>
                </q-item-section>

                <transition-group name="tag" tag="q-item-section" class="column q-gutter-xs items-end content-end">
                  <q-badge v-if="Object.values(infoTraceCount).reduce((a, b) => a + b, 0) > 0" color="info"
                           style="transition: all 200ms;"
                           :label="Object.values(infoTraceCount).reduce((a, b) => a + b, 0)"/>
                  <q-badge v-if="Object.values(warningTraceCount).reduce((a, b) => a + b, 0)" color="warning"
                           style="transition: all 200ms;"
                           :label="Object.values(warningTraceCount).reduce((a, b) => a + b, 0)"/>
                </transition-group>
              </q-item>

              <q-separator></q-separator>

              <q-item v-for="trace in traceTypes" :key="trace.label" clickable @click="targetClick(trace)"
                      :active="filterSwitch[trace.label] && !filterSwitch['Main']"
                      active-class="bg-teal-1 text-secondary">
                <q-item-section avatar>
                  <q-avatar :icon="icons[trace.label]"></q-avatar>
                </q-item-section>

                <q-item-section>
                  <q-item-label>{{ trace.name }}</q-item-label>
                  <q-item-label caption>{{ trace.label }}</q-item-label>
                </q-item-section>

                <transition-group name="tag" tag="q-item-section" class="column q-gutter-xs items-end content-end">
                  <q-badge v-if="infoTraceCount[trace.label] > 0" color="info" key="info"
                           style="transition: all 200ms;"
                           :label="infoTraceCount[trace.label]"/>
                  <q-badge v-if="warningTraceCount[trace.label] > 0" color="warning" key="warning"
                           style="transition: all 200ms;"
                           :label="warningTraceCount[trace.label]"/>
                </transition-group>
              </q-item>
              <q-separator></q-separator>
              <q-item clickable>
                <q-item-section>
                  <q-item-label @click="outputLogFile">导出文件</q-item-label>
                </q-item-section>
              </q-item>
            </q-list>
          </q-card>
          <div style="width:0.5em"></div>
          <q-card class="col" style="height: 45em">
            <q-scroll-area class="full-width full-height column">
              <transition-group
                name="event"
                tag="q-list"
              >
                <q-item v-if="events.length === 0" style="transition: all 200ms;" clickable>
                  <q-item-section avatar>

                    <q-avatar color="grey" text-color="white"
                              icon="link"></q-avatar>
                  </q-item-section>

                  <q-item-section>
                    <q-item-label>正在连接Detour……</q-item-label>
                  </q-item-section>

                </q-item>
                <q-item v-else-if="eventsFilterList().length === 0" style="transition: all 200ms;" clickable
                        key="Undefined">
                  <q-item-section avatar>

                    <q-avatar color="grey" text-color="white"
                              icon="search"></q-avatar>
                  </q-item-section>

                  <q-item-section>
                    <q-item-label>这里什么都没有……</q-item-label>
                  </q-item-section>
                </q-item>
                <q-item v-else v-for="event in eventsFilterList()" :key="event.id" style="transition: all 200ms;"
                        clickable @click="clickEvent(event)">
                  <q-item-section avatar>

                    <q-avatar :color="convertWarningLevel(event['warningLevel'])" text-color="white"
                              :icon="icons[event['eventID']]"></q-avatar>
                  </q-item-section>

                  <q-item-section>
                    <q-item-label>{{ event['event']['Operation'] }}</q-item-label>
                    <q-item-label caption>{{ event['description'] }}</q-item-label>
                  </q-item-section>

                  <q-item-section side class="column q-gutter-xs">
                    <q-item-label caption> [#{{ event.id }}] {{ getTime(event.time) }}</q-item-label>
                    <q-badge v-if="event['warningLevel'] > 0" :color="convertWarningLevel(event['warningLevel'])"
                             :label="'警告: '+ event.warnings.length"/>
                    <q-badge v-else color="info" label="未见异常"/>
                  </q-item-section>
                </q-item>
              </transition-group>
            </q-scroll-area>
          </q-card>
          <div v-if="detailsWindows" style="width:0.5em"></div>
          <q-card v-if="detailsWindows" style="height: 45em;width:300px">
            <q-item>
              <q-item-section avatar>
                <q-avatar :color="convertWarningLevel(detailsEvent['warningLevel'])" text-color="white"
                          :icon="icons[detailsEvent['eventID']]"></q-avatar>
              </q-item-section>

              <q-item-section>
                <q-item-label>{{ detailsEvent['event']['Operation'] }}</q-item-label>
                <q-item-label caption>PID: {{ detailsEvent.pid }}</q-item-label>
                <q-item-label caption>ThreadID: {{ detailsEvent.threadID }}</q-item-label>
              </q-item-section>

              <q-item-section side>
                <q-item-label caption>#{{ detailsEvent.id }}</q-item-label>
              </q-item-section>
            </q-item>
            <q-separator/>
            <q-scroll-area class="full-width column" style="height: 562px">
              <q-list>

                <q-item v-for="(val,key,i) in detailsEvent.eventDescription" :key="i" style="max-width: 300px" clickable
                        v-ripple @click="detailEventClick(val)">
                  <q-item-section>
                    <q-item-label caption>{{ val.description }}</q-item-label>
                    <q-item-label>{{ key }}</q-item-label>
                    <q-item-label caption v-if="typeof val.value == 'string'"
                                  style="word-wrap: break-word;word-break: normal;">{{ val.value }}
                    </q-item-label>
                    <q-item-label caption v-else v-for="item in val.value" :key="item"
                                  style="word-wrap: break-word;word-break: normal;">{{ item }}
                    </q-item-label>
                  </q-item-section>
                  <q-item-section side>
                    <q-badge v-if="key === 'return' " label="返回值" color="primary"></q-badge>
                    <q-badge v-else label="参数" color="info"></q-badge>
                  </q-item-section>
                </q-item>

                <q-separator v-if="detailsEvent.warningLevel > 0"></q-separator>

                <q-item v-for="warning in detailsEvent.warnings" :key="warning.WarningID">
                  <q-item-section side>
                    <q-badge :label="warning.WarningID"
                             :color="warning.type === 'Danger' ? 'negative':'warning'"></q-badge>
                  </q-item-section>
                  <q-item-section>
                    <q-item-label>{{ warning.info }}</q-item-label>
                    <q-item-label caption v-for="(item,i) in warning.extend" :key="i">{{ item }}</q-item-label>
                  </q-item-section>
                </q-item>

                <q-separator v-if="detailsEvent.document"></q-separator>
                <q-item v-if="detailsEvent.document" dense clickable
                        @click="openApiDoc(detailsEvent.document)">
                  <q-item-section>
                    <q-item-label>Microsoft API文档</q-item-label>
                  </q-item-section>
                </q-item>
              </q-list>
            </q-scroll-area>
          </q-card>
        </div>
      </div>
    </div>
    <q-dialog v-model="base64Windows">
      <q-card style="width:60%">
        <q-card-section class="column q-gutter-md">
          <q-input outlined v-model="base64Info.base64" readonly type="textarea" label="Base64"/>
          <q-input outlined v-model="base64Info.Text" readonly type="textarea" label="字符串"/>
          <q-input outlined v-model="base64Info.Binary" readonly type="textarea" label="二进制"/>
        </q-card-section>
      </q-card>
    </q-dialog>
  </q-page>
</template>

<script>
import {defineComponent, ref} from 'vue';
import {date, exportFile} from 'quasar'
import { Base64 } from 'js-base64';

export default defineComponent({
  name: 'PageIndex',
  data: () => {
    return {
      currentTime: new Date(),
      targetFile: 'S:\\softwareSecurityTask\\test\\MFCTest\\Debug\\Target4.exe',
      timer: null,
      startTrace: false,
      firstTrace: false,
      detailsWindows: false,
      runtimeInfo: {
        pid: 0,
        threadID: 0,
      },
      socketConnect: false,
      base64Windows: false,
      base64Info:{
        "base64": "",
        "Text": "",
        "Binary" : "",
      },
      detailsEvent: {
        "event": {
          "Operation": "HeapDestroy",
          "hHeap": 52887552,
          "return": 1
        },
        "eventID": "Heap",
        "pid": 13040,
        "threadID": 3896,
        "time": 1631701840818,
        "warnings": [
          {
            "WarningID": 113,
            "type": "Warning",
            "info": "Destroy Heap时有address未释放",
            "extend": [
              52888840
            ]
          }
        ],
        "warningLevel": 1,
        "description": "成功地摧毁了堆[0x03270000]",
        "eventDescription": {
          "hHeap": {
            "description": "要摧毁的堆的句柄",
            "value": "0x03270000"
          },
          "return": {
            "description": "摧毁结果",
            "value": "成功"
          }
        },
        "id": 8,
        "document": "https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapdestroy"
      },
      traceTypes: [
        {
          "name": "Hook函数",
          "label": "HookFunc"
        },
        {
          "name": "弹框",
          "label": "MessageBox"
        },
        {
          "name": "堆",
          "label": "Heap"
        },
        {
          "name": "文件",
          "label": "File"
        },
        {
          "name": "注册表",
          "label": "Regedit"
        },
        {
          "name": "套接字",
          "label": "Socket"
        }
      ],
      infoTraceCount: {
        "HookFunc": 0,
        "MessageBox": 0,
        "Heap": 0,
        "File": 0,
        "Regedit": 0,
        "Socket": 0,
      },
      warningTraceCount: {
        "HookFunc": 0,
        "Heap": 0,
        "File": 0,
        "Regedit": 0,
        "Socket": 0,
      },
      icons: {
        "HookFunc": "attach_file",
        "MessageBox": "wysiwyg",
        "Heap": "clear_all",
        "File": "folder",
        "Regedit": "app_registration",
        "Socket": "sync_alt"
      },
      filterSwitch: {
        "Main": true,
        "HookFunc": false,
        "MessageBox": false,
        "Heap": false,
        "File": false,
        "Regedit": false,
        "Socket": false,
      },
      events: [
        /*{
          "id": 2,
          "eventID": "MessageBox",
          "time":1631621550*1000,
          "event":{
            "Operation": "MessageBoxA",
            "description": "弹一个框"
          },
          "warningLevel": 2,
          "warnings":[
            {
              "WarningID":501,
              "type": "Danger",
              "info": "弹了一个框"
            },
            {
              "WarningID":511,
              "type": "Warning",
              "info": "弹了一个框"
            }
          ]
        },
        {
          "id": 1,
          "eventID": "MessageBox",
          "time":1631610803*1000,
          "event":{
            "Operation": "MessageBoxA",
            "description": "弹一个框"
          },
          "warningLevel": 0,
          "warnings":[],
        }*/
      ],
      eventSets: [],
      eventCount: 0,
      viewEvent: 0,
    }
  },
  methods: {
    btnStartTrace() {
      let that = this
      this.$axios.get("http://localhost:9013/trace/start", {params: {path: this.targetFile}}).then(res => {
        if (res.data.result) {
          that.eventSets.push(
            {
              events: [],
              runtimeInfo: {
                pid: 0,
                threadID: 0,
              },
              infoTraceCount: {
                "HookFunc": 0,
                "MessageBox": 0,
                "Heap": 0,
                "File": 0,
                "Regedit": 0,
                "Socket": 0,
              },
              warningTraceCount: {
                "HookFunc": 0,
                "Heap": 0,
                "File": 0,
                "Regedit": 0,
                "Socket": 0,
              },
            }
          )
          that.changeViewEvent(this.eventCount)
          that.startTrace = true
          that.firstTrace = true
          that.eventCount++
        } else {
          this.$q.notify({message: "打开目标文件失败", color: "negative"})
        }
      })
    },
    btnStopTrace() {
      this.startTrace = false
      this.$axios.get("http://localhost:9013/trace/stop")
    },
    convertWarningLevel(warningLevel) {
      switch (warningLevel) {
        case 0:
          return 'info'
        case 1:
          return 'warning'
        case 2:
          return 'negative'
      }
      return 'info'
    },
    getTime(time) {
      return date.formatDate(time, 'YYYY/MM/DD HH:mm:ss.SSS')
    },
    targetClick(trace) {
      if (this.filterSwitch['Main']) {
        this.filterSwitch['Main'] = false;
        this.filterSwitch[trace.label] = true;
      } else {
        this.filterSwitch[trace.label] = !this.filterSwitch[trace.label]
      }
    },
    eventsFilterList() {
      return this.events.filter((event) => {
        return this.filterSwitch['Main'] || this.filterSwitch[event['eventID']]
      })
    },
    openApiDoc(url) {
      window.open(url)
    },
    clickEvent(event) {
      if (event.id === this.detailsEvent.id) {
        this.detailsWindows = !this.detailsWindows
      } else {
        this.detailsEvent = event
        this.detailsWindows = true
      }
    },
    changeViewEvent(number) {
      if (!this.startTrace) {
        const thisEvent = this.eventSets[number]
        this.events = thisEvent.events
        this.runtimeInfo = thisEvent.runtimeInfo
        this.infoTraceCount = thisEvent.infoTraceCount
        this.warningTraceCount = thisEvent.warningTraceCount
        this.viewEvent = number
      }
    },
    outputLogFile() {
      let timestamp = new Date().getTime()
      exportFile('log' + timestamp + '.txt', JSON.stringify(this.eventSets))
    },
    detailEventClick(val) {
      if (val.buffer) {
        this.decodeBase64(val.buffer)
        this.base64Windows = true
      }
    },
    decodeBase64(base64val){
      if(Base64.isValid(base64val)){
        this.base64Info =  {
          base64: base64val,
          Text: Base64.decode(base64val),
        }
        let str = ""
        Base64.toUint8Array(base64val).forEach((val)=>{
          if(val <= 16){
            str += '0' + val.toString(16) + ' '
          }else{
            str += val.toString(16) + ' '
          }
        })
        this.base64Info.Binary = str
      }else{
        this.$q.notify({color:"warning",message:"错误的Base64值"})
      }
    }
  },
  mounted() {
    let that = this
    this.$socketIO.on('connect', (res) => {
      that.socketConnect = true
      console.log("startConnect")
    })
    this.$socketIO.on('ConnectInfo', (res) => {
      console.log(res)
    })
    this.$socketIO.on('DetourEvent', (res) => {
      console.log(res)
      res.data['warnings'] = res.data['warnings'].sort((a, b) => {
        return a.WarningID - b.WarningID
      })
      that.events.unshift(res.data)
      if (res.data["warningLevel"] > 0) {
        that.warningTraceCount[res.data["eventID"]] += res.data["warnings"].length
      }
      that.infoTraceCount[res.data["eventID"]]++
      if (res.data["eventID"] === 'HookFunc') {
        if (res.data["event"]["Operation"] === 'StartTrace') {
          that.pid = res.data['pid']
          that.threadID = res.data['threadID']
        } else if (res.data["event"]["Operation"] === 'StopTrace') {
          that.startTrace = false
        }
      }
    })
    this.$socketIO.on('disconnect', (res) => {
      that.socketConnect = false
      console.log("EndConnect")
    })
    //this.decodeBase64("SGVsbG8gV29ybGQh")
  }
})
</script>

<style>
.event-enter-from, .event-leave-to {
  opacity: 0;
  transform: translateY(-20px)
}

.event-leave-active {
  position: absolute;
  width: 100%
}

.tag-enter-from, .tag-leave-to {
  opacity: 0;
  transform: translateX(20px)
}

.tag-leave-active {
  position: absolute;
}

.hint-enter-from {
  opacity: 0;
  transform: translateY(20px)
}

.hint-leave-to {
  opacity: 0;
  transform: translateY(-40px)
}

.hint-leave-from {
  transform: translateY(-20px)
}

.hint-leave-active {
  position: absolute;
}
</style>
