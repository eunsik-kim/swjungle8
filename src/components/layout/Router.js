import React from "react";
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import Layout from './Layout'
import Home from "../../pages/Home";
import Problem from "../../pages/Problems";
import Solve from "../../pages/Solve";
import Algorithm from "../../pages/Algorithms";
import Login from "./Login";
import SignUp from "./SignUp";
import ProblemSolve from "pages/ProblemSolve";
import AlgorithmDetail from "pages/AlgorithmDetail";

const Router = () => {
  return (
    <BrowserRouter>
      <Layout>
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/problem" element={<Problem />} />
          <Route path="/problem/:id" element={<ProblemSolve />} />
          <Route path="/solve" element={<Solve />} />
          <Route path="/algorithm" element={<Algorithm />} />
          <Route path="/algorithm/:id" element={<AlgorithmDetail />} />        
          <Route path="/login" element={<Login />} />
          <Route path="/signup" element={<SignUp />} />
        </Routes>
      </Layout>
    </BrowserRouter>
  )
}

export default Router;