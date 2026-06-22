# Library Management System - GitHub Pages

This is the GitHub Pages portfolio website for the Library Management System project.

## 🚀 Live Website

Visit the live website at: `https://alizahusman006.github.io/library-management/`

## 📁 Structure

```
docs/
├── index.html      # Main portfolio landing page
├── styles.css      # Portfolio website styles
├── script.js       # Portfolio navigation and animations
├── app.html        # Interactive demo page
├── app.css         # Demo application styles
├── app.js          # Demo application logic (localStorage)
└── README.md       # This file
```

## ✨ Features

- **Modern Portfolio Design**: Beautiful landing page showcasing the project
- **Live Demo**: Interactive demo version using browser localStorage
- **Responsive Design**: Works perfectly on all devices
- **Smooth Animations**: Scroll animations and transitions
- **Feature Highlights**: Showcases all key features and OOP concepts

## 🛠️ Technologies Used

- HTML5
- CSS3 (with animations and responsive design)
- JavaScript (ES6+)
- Font Awesome Icons
- LocalStorage API (for demo)

## 📝 Setup Instructions

### For GitHub Pages:

1. **Enable GitHub Pages** in your repository settings:
   - Go to your repository on GitHub
   - Click on "Settings" tab
   - Scroll down to "Pages" section
   - Under "Source", select "Deploy from a branch"
   - Select branch: `main` (or `master`)
   - Select folder: `/docs`
   - Click "Save"

2. **Wait for deployment** (usually takes 1-2 minutes)

3. **Visit your website** at: `https://alizahusman006.github.io/library-management/`

### For Local Testing:

1. Navigate to the docs folder:
   ```bash
   cd library-management/docs
   ```

2. Start a local server (choose one):
   ```bash
   # Using Python 3
   python3 -m http.server 8000
   
   # Using Python 2
   python -m SimpleHTTPServer 8000
   
   # Using Node.js (if you have http-server installed)
   npx http-server
   ```

3. Open browser and go to: `http://localhost:8000`

## 🎯 Demo Features

The demo version (`app.html`) includes:

- ✅ Add Books, Magazines, and DVDs
- ✅ Register library members
- ✅ Borrow and return resources
- ✅ Filter resources by type
- ✅ Real-time statistics
- ✅ Late fee calculation
- ✅ Data persistence (localStorage)
- ✅ Clear all data option

## 📊 Project Information

- **Author**: Alizah Usman
- **GitHub**: [@alizahusman006](https://github.com/alizahusman006)
- **Email**: alizahusman115@gmail.com
- **Original Project**: [library-management](https://github.com/alizahusman006/library-management)

## 🎓 OOP Concepts Demonstrated

1. **Inheritance**: `LibraryResource` → `Book`, `Magazine`, `DVD`
2. **Polymorphism**: Virtual functions for different resource types
3. **Encapsulation**: Private members with public getters/setters
4. **Abstraction**: Base class provides abstract interface

## 📄 License

This project is open source and available for educational purposes.

## 🙏 Acknowledgments

Built as a 2nd semester OOP assignment to demonstrate:
- Object-Oriented Programming principles
- Full-stack development with C++
- Modern web interface design
- GitHub Pages deployment

---

**Note**: The demo version uses localStorage for data persistence. The full version with C++ backend provides better performance and file-based data storage.