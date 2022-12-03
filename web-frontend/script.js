import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.14.0/firebase-app.js'
import { getFirestore, doc, getDoc, setDoc } from 'https://www.gstatic.com/firebasejs/9.14.0/firebase-firestore.js'

const toggleModal = () => {
    document.querySelector('#modal-overlay').classList.toggle('hidden')
    document.querySelector('.qr').classList.toggle('hidden')

    let min, sec;
    const qrCode = new QRCodeStyling({
          width: 250,
          height: 250,
          data: `${localStorage.getItem('roll')}`,
          image: "./assets/raz0229_small.gif",
          dotsOptions: {
            color: "#000",
            type: "square"
          },
          backgroundOptions: {
            color: "#FFFFFF",
          }
        });
    qrCode.append(document.querySelector(".qr-code"));

    setInterval(()=>{
        const n = new Date().getTime() - (parseInt(localStorage.getItem('created'))*1000)
        const seconds = n/1000

        min = 30 - Math.floor(seconds/60)
        sec = 60 - Math.floor(seconds%60)

        if (min < 0) {
            min = '00';
            sec = '00';
        }

        document.querySelector('#min').innerText = min;
        document.querySelector('#sec').innerText = sec;
        

    },1000)
}

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

const N = new Date().getTime() - (parseInt(localStorage?.getItem('created'))*1000)
const SECONDS = N/1000
const MIN = 30 - Math.floor(SECONDS/60)

if (MIN > 0) {
    if (localStorage.getItem('roll'))
    toggleModal();
} else {
    localStorage.removeItem('roll')
    localStorage.removeItem('created')
}

let selected, roll;

let year = document.querySelector(".cp-text #year");
year.textContent = new Date().getFullYear();

document.querySelector('ul').addEventListener('click', (x) => {
    selected = x.target.textContent
    document.querySelector('.span').innerText = selected;
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
            let created = Math.floor((new Date().getTime())/1000);
            await setDoc(docRef, {
                name: docSnap.data().name,
                reason: selected,
                created });

            localStorage.setItem("created", created);
            localStorage.setItem("roll", roll.trim().slice(-4));

            toggleModal();

        } else {
            document.querySelector('.error').style.display = 'block'
        }

        document.querySelector('.lds-ellipsis').classList.toggle('hidden')
        document.querySelector('.submit-text').classList.toggle('hidden')
        
    }
    
})
