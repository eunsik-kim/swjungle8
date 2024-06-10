import ReactMarkdown from 'react-markdown';
import remarkGfm from 'remark-gfm'; 

function MarkdownRenderer({ children }) {
  return <ReactMarkdown remarkPlugins={[remarkGfm]}>{children}</ReactMarkdown>;
}

export default MarkdownRenderer;