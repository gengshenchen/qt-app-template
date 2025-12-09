import logo from './logo.svg';
import './App.css';

function App() {
  const sendMessageToQt = () => {
    if (window.CallBridge) {
      window.CallBridge.invoke("test", "Hello from JS");
    } else {
      console.log("CallBridge not ready");
    }
  };

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.js</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
        <button onClick={sendMessageToQt}>Send Message to Qt</button>
      </header>
    </div>
  );
}

export default App;
