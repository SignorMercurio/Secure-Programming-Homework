<template>
  <q-page class="flex">
    <q-layout view="lhr lpR lFr">
      <q-header class="bg-blue-grey-10 text-white">
        <q-toolbar>
          <q-toolbar-title>
            <q-item>
              <q-item-section>
                <q-item-label>{{ friend }}</q-item-label>
                <q-item-label caption
                  >Hello, this is {{ friend }} speaking...</q-item-label
                >
              </q-item-section>
            </q-item>
          </q-toolbar-title>
        </q-toolbar>
      </q-header>

      <q-drawer show-if-above v-model="left" side="left" behavior="desktop">
        <q-list>
          <q-item clickable v-ripple>
            <q-item-section avatar>
              <q-avatar color="red">{{ friend[0] }}</q-avatar>
            </q-item-section>
            <q-item-section>
              <q-item-label>{{ friend }}</q-item-label>
              <q-item-label caption>Let's chat!</q-item-label>
            </q-item-section>
          </q-item>
        </q-list>
      </q-drawer>

      <q-page-container>
        <q-scroll-area
          ref="scrollArea"
          :visible="false"
          style="height: calc(100vh - 180px);"
          :thumb-style="thumbStyle"
          :bar-style="barStyle"
        >
          <div class="q-pa-lg justify-center">
            <q-chat-message
              :label="new Date().toLocaleDateString()"
            ></q-chat-message>
            <q-chat-message
              text-sanitize
              v-for="(item, index) in msgs"
              :key="index"
              :sent="item.data.to === friend"
              :stamp="item.stamp"
            >
              {{ item.msg }}
              <q-btn
                size="xs"
                flat
                round
                icon="more_horiz"
                @click="showDetail(item.data)"
              ></q-btn>
            </q-chat-message>
          </div>
        </q-scroll-area>
      </q-page-container>

      <q-footer class="bg-blue-grey-10 text-white">
        <q-toolbar>
          <q-form class="full-width">
            <q-input
              borderless
              placeholder="Write a message..."
              v-model="message"
            >
              <template v-slot:append>
                <q-btn
                  type="submit"
                  flat
                  round
                  icon="send"
                  color="primary"
                  @click="send"
                ></q-btn>
              </template>
            </q-input>
          </q-form>
        </q-toolbar>
      </q-footer>
    </q-layout>
    <q-dialog v-model="detail">
      <q-card class="dialog">
        <q-card-section>
          <div class="text-h6">Message Detail</div>
        </q-card-section>
        <q-card-section>
          <q-input
            readonly
            autogrow
            outlined
            type="textarea"
            v-model="detailData"
          ></q-input>
        </q-card-section>
        <q-card-actions align="right">
          <q-btn flat label="OK" color="primary" v-close-popup />
        </q-card-actions>
      </q-card>
    </q-dialog>
  </q-page>
</template>

<script>
import {
  randomBytes,
  publicEncrypt,
  createCipheriv,
  privateDecrypt,
  createDecipheriv,
  createSign,
  createVerify,
} from 'crypto'
import { readFileSync } from 'fs'
import io from 'src/statics/socket.io'
import { ws_url } from 'src/statics/config.js'

export default {
  data() {
    return {
      left: true,
      me: this.$store.getters.getUserInfo.username,
      friend: '',
      message: '',
      aesKeyEnc: '',
      socket: null,
      msgs: [],
      detail: false,
      detailData: '',
      thumbStyle: {
        right: '4px',
        borderRadius: '5px',
        backgroundColor: '#027be3',
        width: '5px',
        opacity: 0.75,
      },
      barStyle: {
        right: '2px',
        borderRadius: '9px',
        backgroundColor: '#027be3',
        width: '9px',
        opacity: 0.2,
      },
    }
  },

  mounted() {
    this.friend = this.me === 'Alice' ? 'Bob' : 'Alice'
    this.socket = io(ws_url)
    this.socket.on('connect', () => {
      console.log('ws connected')
    })
    this.socket.on('json', (msg) => {
      console.log(msg)
      if (msg.to === this.me) {
        this.recv(msg)
      }
    })
  },
  methods: {
    scroll2Bottom() {
      const scrollArea = this.$refs.scrollArea
      scrollArea.setScrollPosition(
        scrollArea.getScrollTarget().scrollHeight,
        100
      )
    },
    async send() {
      if (this.message === '') return
      const rsaPk = await this.getPubkey('RSA')
      const aesKey = this.getAESkeyEnc(rsaPk)
      const msgEnc = this.encryptMsg(aesKey)
      const data = {
        to: this.friend,
        msgEnc: msgEnc,
        aesKeyEnc: this.aesKeyEnc,
        sig: this.sign(),
      }
      console.log(data)
      this.socket.emit('msg', data)
      this.msgs.push({
        msg: this.message,
        data: data,
        stamp: this.$now(),
      })
      this.message = ''
      this.scroll2Bottom()
    },
    async recv(data) {
      const { sig, aesKeyEnc, msgEnc } = data
      const dsaPk = await this.getPubkey('DSA')
      const result = this.verify(dsaPk, sig, aesKeyEnc)
      if (!result) {
        this.$fail('New message verify failed...')
        return false
      }
      this.$success('New message verify')
      let rsaSk = readFileSync(`${this.me}_RSAsk.pem`, 'utf8')
      let aesKey = this.getAESkeyDec(rsaSk, aesKeyEnc)
      let msg = this.decryptMsg(msgEnc, aesKey)
      this.msgs.push({
        msg: msg,
        data: data,
        stamp: this.$now(),
      })
      this.scroll2Bottom()
    },
    showDetail(data) {
      this.detail = true
      this.detailData = JSON.stringify(data, null, 4)
    },
    getPubkey(type) {
      return this.$axios
        .post('getPubkey', {
          username: this.friend,
          type: type,
        })
        .then((res) => {
          if (res) {
            return res.data.data.pubkey
          }
        })
    },
    getAESkeyEnc(rsaPk) {
      let aesKey = randomBytes(32)
      this.aesKeyEnc = publicEncrypt(rsaPk, aesKey).toString('hex')
      //writeFileSync('AESkey.enc', this.aesKeyEnc)
      return aesKey
    },
    getAESkeyDec(rsaSk, aesKeyEnc) {
      return privateDecrypt(
        { key: rsaSk, passphrase: 'chatroom' },
        Buffer.from(aesKeyEnc, 'hex')
      )
    },
    encryptMsg(aesKey) {
      let cipher = createCipheriv('aes-256-cbc', aesKey, Buffer.alloc(16, 0))
      let msgEnc = cipher.update(this.message, 'utf8', 'hex')
      msgEnc += cipher.final('hex')

      return msgEnc
    },
    decryptMsg(msgEnc, aesKey) {
      let decipher = createDecipheriv(
        'aes-256-cbc',
        aesKey,
        Buffer.alloc(16, 0)
      )
      let msg = decipher.update(msgEnc, 'hex', 'utf8')
      msg += decipher.final('utf8')

      return msg
    },
    sign() {
      const dsaSk = readFileSync(`${this.me}_DSAsk.pem`)
      const sign = createSign('SHA1')
      sign.write(this.aesKeyEnc)
      sign.end()
      const sig = sign.sign(
        {
          key: dsaSk,
          passphrase: 'chatroom',
        },
        'hex'
      )
      /*
      let sig = await openssl(
        `dgst -sign ${this.me}_DSAsk.pem -passin pass:chatroom -out sig.sign AESkey.enc`
      )
        .then(() => {
          return readFileSync('sig.sign')
        })
        .catch(err => console.error(err))
      */

      return sig
    },
    verify(dsaPk, sig, aesKeyEnc) {
      const verify = createVerify('SHA1')
      verify.write(aesKeyEnc)
      verify.end()
      let result = verify.verify(dsaPk, sig, 'hex')
      /*
      let result = await openssl(
        `dgst -verify ${this.friend}_DSApk.pem -signature recv_sig.sign recv_AESkey.enc`
      )
        .then(res => {
          return res.split(' ')[1].replace(/[\r\n]$/g, '') === 'OK'
        })
        .catch(err => console.error(err))*/

      return result
    },
  },
}
</script>
