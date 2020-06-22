<template>
  <q-page class="flex flex-center">
    <q-card class="login-card">
      <q-card-section>
        <div class="text-center q-gutter-y-sm">
          <img src="~assets/icon.png" alt="icon" />
          <div class="text-h6">Chatroom</div>
        </div>
      </q-card-section>

      <q-tab-panel name="login">
        <q-form @submit="login" class="q-pa-md q-gutter-md">
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

          <div>
            <q-btn
              push
              class="full-width"
              label="Login"
              type="submit"
              color="primary"
            />
          </div>
        </q-form>
        <div class="flex">
          <q-space></q-space>
          <q-btn label="Register" flat color="primary" to="register" />
        </div>
      </q-tab-panel>
    </q-card>
  </q-page>
</template>

<script>
import { createHash } from 'crypto'

export default {
  data() {
    return {
      username: '',
      password: ''
    }
  },
  methods: {
    login() {
      const sha256 = createHash('sha256')
      sha256.update(this.password)

      this.$axios
        .get('token', {
          auth: {
            username: this.username,
            password: sha256.digest('hex')
          }
        })
        .then(res => {
          if (res) {
            this.$store.dispatch('Login', res.data.data)
            this.$success('Login')
            setTimeout(() => {
              this.$router.push('/chat')
            }, 500)
          }
        })
    }
  }
}
</script>
