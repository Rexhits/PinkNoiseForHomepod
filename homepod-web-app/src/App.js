import './App.css';


const sendReq = async(id) =>
{
  await fetch(`api/${id}`, {
    method: 'POST'
  });
};

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <h1>Brown Noise</h1>
        <button className="transport" onClick={() => sendReq('play')}>Start</button>
        <button className="transport" onClick={() => sendReq('stop')}>Stop</button>
      </header>
    </div>
  );
}

export default App;
