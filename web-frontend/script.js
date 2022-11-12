import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.14.0/firebase-app.js'
import { getFirestore, doc, getDoc, setDoc } from 'https://www.gstatic.com/firebasejs/9.14.0/firebase-firestore.js'

      const firebaseConfig = {
    apiKey: "AIzaSyB4hl9a1RPB_MmuqPm_zNmO49Y20qSf9e4",
    authDomain: "stoked-sun-304807.firebaseapp.com",
    projectId: "stoked-sun-304807",
    storageBucket: "stoked-sun-304807.appspot.com",
    messagingSenderId: "622344556987",
    appId: "1:622344556987:web:2018a94371c8eeef90f49e",
    measurementId: "G-9WXYEFVCQ0"
};

const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

let selected, roll;

let year = document.querySelector(".cp-text #year");
year.textContent = new Date().getFullYear();

document.querySelector('ul').addEventListener('click', (x) => {
    selected = x.target.textContent
    document.querySelector('span').innerText = selected;
    document.querySelector('#touch').checked = false
})

document.querySelector('.submit').addEventListener('click', async (e)=> {
    e.preventDefault();
    roll = document.querySelector('#roll').value
    if (!selected)
        document.querySelector('nav').style.borderColor = '#b00';
    else if (!roll) {
        document.querySelector('#roll').style.borderWidth = '1px';
        document.querySelector('#roll').style.borderColor = '#b00';
    }
    else {
        document.querySelector('.lds-ellipsis').classList.toggle('hidden')
        document.querySelector('.submit-text').classList.toggle('hidden')
        
        const docRef = doc(db, "students", roll.trim().slice(-4));
        const docSnap = await getDoc(docRef);

        if (docSnap.exists()) {
            await setDoc(docRef, {
                name: docSnap.data().name,
                reason: selected,
                created: Math.floor((new Date().getTime())/1000) });

        } else {
            console.log("No such document!");
            document.querySelector('.error').style.display = 'block'
        }

        document.querySelector('.lds-ellipsis').classList.toggle('hidden')
        document.querySelector('.submit-text').classList.toggle('hidden')
        
    }
    
})