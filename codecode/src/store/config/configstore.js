import { configureStore } from "@reduxjs/toolkit";
import { dummySlice } from "../modules/dummy"

const store = configureStore({
    reducer: {
        dummy: dummySlice.reducer,
    }
});

export default store;