# Library Management System

A full-stack Library Management System built with **C++ backend** and **HTML/CSS/JavaScript frontend**. This project demonstrates Object-Oriented Programming (OOP) concepts including inheritance, polymorphism, and encapsulation.

## Features

- **Add Resources**: Books, Magazines, and DVDs with unique attributes
- **Register Members**: Add new library members with auto-generated IDs
- **Borrow/Return**: Members can borrow and return resources
- **Track Availability**: Real-time status of all resources
- **Late Fee Calculation**: Automatic late fee calculation based on resource type
- **Data Persistence**: All data is saved to file and loaded on startup
- **Responsive UI**: Clean, modern web interface

## Project Structure

```
library-management/
├── backend/
│   ├── library.h      # Core library classes (OOP implementation)
│   └── server.cpp     # HTTP server and API endpoints
├── frontend/
│   ├── index.html     # Main HTML page
│   ├── styles.css     # CSS styling
│   └── script.js      # JavaScript for API communication
├── library_data.txt   # Data storage file (auto-generated)
└── README.md          # This file
```

## OOP Concepts Used

### 1. Inheritance
- `LibraryResource` (base class)
  - `Book` (derived class)
  - `Magazine` (derived class)
  - `DVD` (derived class)

### 2. Polymorphism
- Virtual functions: `getType()`, `getExtraInfo()`, `calculateLateFee()`, `toJSON()`
- Dynamic method dispatch for different resource types

### 3. Encapsulation
- Private data members with public getter/setter methods
- Data hiding in `LibraryMember` and `Library` classes

### 4. Abstraction
- Base class `LibraryResource` provides abstract interface
- `Library` class manages complex operations internally

## Classes Overview

### LibraryResource (Base Class)
- Common attributes: ID, title, author, availability
- Virtual functions for polymorphic behavior

### Book
- Additional attributes: ISBN, page count
- Late fee: Rs. 5 per day

### Magazine
- Additional attributes: Issue number
- Late fee: Rs. 3 per day

### DVD
- Additional attributes: Duration (minutes)
- Late fee: Rs. 10 per day

### LibraryMember
- Attributes: Member ID, name, borrowed resources
- Methods: borrow, return, calculate late fees

### Library
- Manages all resources and members
- Handles data persistence (save/load from file)
- Provides JSON serialization for API

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/resources` | GET | Get all resources |
| `/api/resources` | POST | Add new resource |
| `/api/members` | GET | Get all members |
| `/api/members` | POST | Register new member |
| `/api/borrow` | POST | Borrow a resource |
| `/api/return` | POST | Return a resource |
| `/api/stats` | GET | Get statistics |

## 📁 Project Setup

This is a full-stack application with a C++ backend. To run it:

1. Clone this repository
2. Follow the "How to Run Locally" instructions below
3. The application will be available at `http://localhost:8080`

**Note:** Due to the C++ backend, this project cannot be hosted on GitHub Pages (which only supports static websites). You must run it locally or deploy to a platform that supports C++ servers like Render.com or Railway.app.

## How to Run Locally

### Prerequisites
- GCC/G++ compiler
- Terminal/Command prompt

### Compilation

```bash
# Navigate to backend directory
cd library-management/backend

# Compile the server
g++ -o server server.cpp -std=c++11

# Run the server
./server
```

### Access the Application

1. Start the backend server (runs on port 8080)
2. Open your browser and go to: `http://localhost:8080`
3. Use the web interface to manage the library

## Usage Guide

### Adding a Resource
1. Select resource type (Book/Magazine/DVD)
2. Enter title and author
3. Fill in type-specific details (ISBN & pages for books, etc.)
4. Click "Add Resource"

### Registering a Member
1. Enter member name
2. Click "Register Member"
3. Note the auto-generated Member ID

### Borrowing a Resource
1. Enter your Member ID
2. Enter the Resource ID you want to borrow
3. Click "Borrow"

### Returning a Resource
1. Enter your Member ID
2. Enter the Resource ID you want to return
3. Click "Return"

## Data Storage

The system uses a simple text file (`library_data.txt`) for data persistence:

```
RESOURCES:
1|Book|The C++ Programming|Bjarne Stroustrup|ISBN:9780321568142|Pages:912|1
2|Magazine|National Geographic|Various|Issue:202|1

MEMBERS:
1|Alice Johnson
2|Bob Smith

BORROWED:
1|1|Mon Jun 23 12:00:00 2025
```

## Late Fee Calculation

- **Books**: Rs. 5 per day
- **Magazines**: Rs. 3 per day
- **DVDs**: Rs. 10 per day

## Future Enhancements

- Search functionality
- Due date tracking
- Email notifications
- User authentication
- Fine payment system
- Book reservation system
- Admin dashboard with charts

## Technologies Used

- **Backend**: C++ with socket programming
- **Frontend**: HTML5, CSS3, JavaScript (ES6+)
- **Architecture**: Client-Server with REST API
- **Data Format**: JSON for API, text file for storage

## Author

**Alizah Usman**
- GitHub: [@alizahusman006](https://github.com/alizahusman006)
- Email: alizahusman115@gmail.com
- LinkedIn: [Add your LinkedIn here](https://linkedin.com/in/your-profile)

## License

This project is open source and available for educational purposes.

## Acknowledgments

This project was built as a 2nd semester OOP assignment to demonstrate:
- Object-Oriented Programming principles
- Full-stack development with C++
- Simple HTTP server implementation
- Modern web interface design

## Why Not GitHub Pages?

GitHub Pages only hosts **static websites** (HTML, CSS, JavaScript). This project requires a **C++ backend server** to:
- Process API requests
- Manage library data
- Handle file operations

Therefore, this project must be run locally or deployed to a server that supports C++ applications.

## Deployment Options

If you want to deploy this online:

1. **Render.com (Free tier available):**
   - Fork this repository
   - Sign up at [render.com](https://render.com)
   - Create a new "Web Service"
   - Connect your GitHub repository
   - Set build command: `cd backend && g++ -std=c++11 -Wall -Wextra -o server server.cpp`
   - Set start command: `cd backend && ./server`
   - Deploy!

2. **Railway.app (Free tier available):**
   - Sign up at [railway.app](https://railway.app)
   - Create new project from GitHub repo
   - Deploy with one click

3. **Run Locally (Recommended for testing):**
   - Follow the instructions above to run on your computer

---

**Note**: This is a student project meant for learning purposes. For production use, consider using a proper web framework and database system.
