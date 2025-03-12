<template>
    <VContainer class="fill-height " align="center" justify="center">
        <v-card
            class="py-8 px-6 text-center mx-auto ma-4"
            elevation="12"
            max-width="400"
            width="100%"
        > 
            <h3 class="text-h6 mb-4">Combination</h3>
            <div class="text-body-2">Enter Your 4-Digit Passcode</div>
            <v-sheet color="surface">
                <v-otp-input
                    v-model="passcode"
                    variant="solo"
                    :length="4"
                ></v-otp-input>
            </v-sheet>
            <v-btn
                class="my-4"
                color="purple"
                height="40"
                text="Submit"
                variant="flat"
                width="70%"
                @click="setPasscode();"
            ></v-btn>
        </v-card>
    </VContainer>
    
</template>
    
<script setup>
// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useAppStore } from "@/store/appStore";



// VARIABLES
const router      = useRouter();
const route       = useRoute();
const AppStore    = useAppStore(); 
const passcode    = ref("");


// COMPUTED PROPERTIES

// WATCHERS
// watch(fullname,  (name) => {
//     // console.log(name);  
//     clearTimeout(nameID); // prevent errant multiple timeouts from being generated
//     typing.value = "typing..."
//     nameID = setTimeout(() => {typing.value = ""},1000);
// });

// FUNCTIONS
const setPasscode = async ()=>{
    const data = await AppStore.setCombination(passcode);
}

onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
});

</script>
    
<style scoped>
/** CSS STYLE HERE */

</style>