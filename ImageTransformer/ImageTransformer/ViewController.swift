//
//  ViewController.swift
//  ImageTransformer
//
//  Created by Miraslau Parafeniuk on 23.03.26.
//

import Cocoa

class ViewController: NSViewController {
    
    let imageView: NSImageView = {
        let imageView = NSImageView(frame: NSMakeRect(50,50,975,585))
        imageView.translatesAutoresizingMaskIntoConstraints = false
        return imageView
    }()
    
    let label: NSTextField = {
        let lbl = NSTextField(frame: NSMakeRect(400,10,400,30))
        lbl.isSelectable = false
        lbl.isEditable = false
        lbl.isBezeled = false
        lbl.textColor = .black
        lbl.font = .systemFont(ofSize: 20)
        return lbl
    }()
    
    let goButton: NSButton = {
        let btn = NSButton(frame: NSMakeRect(5,5,70,40))
        btn.bezelStyle = .push
        btn.title = "GO!"
        return btn
    }()
    
    var matrix: [[Int]] = []
    var step = 0

    override func viewDidLoad() {
        super.viewDidLoad()

        view.addSubview(imageView)
        view.addSubview(label)
        view.addSubview(goButton)
        
        label.stringValue = "Step #\(step)"
        goButton.target = self
        goButton.action = #selector(go)
        
        let url = URL(fileURLWithPath: "/Users/miraslau/Downloads/Project Euler secret problem image.png")
        if let data = try? Data(contentsOf: url) {
            imageView.image = NSImage(data: data)
        }

        matrix = try! grayscaleMatrix(from: url)
        print("Height =", matrix.count)
        print("Width =", matrix.first?.count ?? 0)
        print(matrix[0][0]) // value of top-left pixel, from 0 to 255
    }
    
    @objc func go() {
        if step < 10 {
            step = 999999999990
            matrix = secretAfterNStepsMod7(matrix, step)
        }
        else {
            step += 1
            matrix = newMatrix(matrix)
        }
        print("\(step) Matrix loaded")
        imageView.image = try! image(from: matrix, scale: 36)
        print("\(step) Image generated")
        label.stringValue = "Step #\(step)"
    }

    func newMatrix(_ A: [[Int]]) -> [[Int]] {
        var k = 0
        let N = A.count; let M = A[0].count
        var B = A
        for i in 0..<A.count {
            for j in 0..<A[i].count {
                var s = 0
                let t1 = i > 0 ? A[i-1][j] : A[N-1][j]
                let t2 = j > 0 ? A[i][j-1] : A[i][M-1]
                let t3 = i < N-1 ? A[i+1][j] : A[0][j]
                let t4 = j < M-1 ? A[i][j+1] : A[i][0]
                if t1 == t2 && t1 == t3 && t1 == t4 {
                    k += 1
                }
                s += t1+t2+t3+t4
                B[i][j] = actualPixel(s)
            }
        }
        print(k)
        return B
    }
    
    func actualPixel(_ s: Int) -> Int {
        (s%7)
    }
    
    func secretAfterNStepsMod7(_ input: [[Int]], _ N: Int) -> [[Int]] {
        let h = input.count
        let w = input[0].count

        // Работаем сразу по mod 7
        var current = input.map { row in
            row.map { (($0 % 7) + 7) % 7 }
        }

        var next = Array(
            repeating: Array(repeating: 0, count: w),
            count: h
        )

        // 10^12 = 132150634516021 (base 7)
        var digits: [Int] = [] // but here we will obtain digits in reverse order
        var n = N
        while n > 0 {
            digits.append(n % 7)
            n /= 7
        }

        var dx = 1 % w   // 7^0 mod w
        var dy = 1 % h   // 7^0 mod h

        for a in digits {
            for _ in 0..<a {
                for y in 0..<h {
                    let yUp = (y - dy + h) % h
                    let yDown = (y + dy) % h

                    for x in 0..<w {
                        let xLeft = (x - dx + w) % w
                        let xRight = (x + dx) % w

                        next[y][x] =
                            (current[y][xLeft]
                           + current[y][xRight]
                           + current[yUp][x]
                           + current[yDown][x]) % 7
                    }
                }

                swap(&current, &next)
            }

            dx = (dx * 7) % w
            dy = (dy * 7) % h
        }

        return current
    }
}

