import React from 'react'

function Loader() {
  return (
    <div>
        <div>Cagando...</div>
        <progress className="progress progress-success w-56"></progress>
    </div>
  )
}

export default Loader