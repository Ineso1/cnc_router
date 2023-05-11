import React from 'react'

function Header() {
  return (
    <div className="navbar absolute bg-primary-focus text-primary-content z-50">
      <div className="navbar-start">
        <a className="btn btn-ghost normal-case text-xl">NombreProyecto</a>
      </div>
      <div className="navbar-end">
        <a className="btn border-success bg-info-content">Start process</a>
      </div>
    </div>
  )
}

export default Header