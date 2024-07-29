import GetProblem from "services/GetProblem";

const GetRandomProblem = async (is_success, is_review) => {
  const data = await GetProblem("");
  console.log(data);
  const problemIds = data && data.data ? data.data.filter(post => {
    return post['is_success'] === !is_success && post['is_review'] === is_review;
  }).map(post => post.id) : null; 

  return problemIds ? problemIds[Math.floor(Math.random() * problemIds.length)] : 0;
}
export default GetRandomProblem;