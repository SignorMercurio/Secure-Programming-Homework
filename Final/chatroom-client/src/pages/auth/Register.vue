<template>
  <q-page class="flex flex-center">
    <q-card class="login-card">
      <q-card-section>
        <div class="text-center q-gutter-y-sm">
          <img src="~assets/icon.png" alt="icon" />
          <div class="text-h6">Chatroom</div>
        </div>
      </q-card-section>

      <q-tab-panel name="reg">
        <q-form @submit="reg" class="q-pa-md q-gutter-md">
          <q-input
            v-model="username"
            dense
            label="Account"
            lazy-rules
            :rules="[val => !!val || '* Required']"
          />

          <q-input
            type="password"
            dense
            v-model="password"
            label="Password"
            lazy-rules
            :rules="[val => !!val || '* Required']"
          />

          <q-input
            type="password"
            dense
            v-model="confirmPassword"
            label="Confirm Password"
            lazy-rules
            :rules="[
              val => val === this.password || '! Password does not match'
            ]"
          />

          <div>
            <q-btn
              push
              class="full-width"
              label="Register"
              type="submit"
              color="primary"
            />
          </div>
        </q-form>
      </q-tab-panel>
    </q-card>
  </q-page>
</template>

<script>
import { writeFileSync } from 'fs'
import { generateKeyPairSync, createHash, getCurves } from 'crypto'

export default {
  data() {
    return {
      username: '',
      password: '',
      confirmPassword: '',
      rsaPk: '',
      dsaPk: ''
    }
  },
  methods: {
    async reg() {
      const sha256 = createHash('sha256')
      sha256.update(this.password)

      this.RSAkeyGen()
      this.DSAkeyGen()

      this.$axios
        .post('users/reg', {
          username: this.username,
          password: sha256.digest('hex'),
          rsaPk: this.rsaPk,
          dsaPk: this.dsaPk
        })
        .then(res => {
          if (res) {
            this.$success('Register')
            setTimeout(() => {
              this.$router.push('/auth/login')
            }, 500)
          }
        })
    },
    RSAkeyGen() {
      const { publicKey, privateKey } = generateKeyPairSync('rsa', {
        modulusLength: 2048,
        publicKeyEncoding: {
          type: 'spki',
          format: 'pem'
        },
        privateKeyEncoding: {
          type: 'pkcs8',
          format: 'pem',
          cipher: 'aes-256-cbc',
          passphrase: 'chatroom'
        }
      })
      this.rsaPk = publicKey
      writeFileSync(`${this.username}_RSAsk.pem`, privateKey)
    },
    DSAkeyGen() {
      const { privateKey, publicKey } = generateKeyPairSync('ec', {
        namedCurve: 'prime256v1',
        publicKeyEncoding: {
          type: 'spki',
          format: 'pem'
        },
        privateKeyEncoding: {
          type: 'pkcs8',
          format: 'pem',
          cipher: 'aes-256-cbc',
          passphrase: 'chatroom'
        }
      })
      this.dsaPk = publicKey
      writeFileSync(`${this.username}_DSAsk.pem`, privateKey)
      /*
      return openssl('dsaparam -out dsaparam.pem 1024').then(async () => {
        let pk = await openssl(
          'gendsa -out sk.pem -aes256 -passout pass:chatroom ./dsaparam.pem'
        ).then(async () => {
          openssl(
            `pkcs8 -topk8 -inform PEM -in ./sk.pem -outform PEM -out ${this.username}_DSAsk.pem -passin pass:chatroom -passout pass:chatroom`
          )
          let pk = await openssl(
            'dsa -in ./sk.pem -pubout -passin pass:chatroom'
          )
          return pk
        })
        return pk
      })*/
    }
  }
}
</script>
